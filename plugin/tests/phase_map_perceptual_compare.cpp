/* SPDX-License-Identifier: GPL-3.0-only */
/* Compare two phase_map perceptual dumps. The reference is the current F/SEG
 * read (mode 0); each candidate is a direct-read table. The comparator first
 * requires the map geometry and the stochastic timeline to be identical, and
 * the exact 4096 table (mode 1) to be sample-identical. It then measures, per
 * scenario:
 *
 *   - per-frame log-spectral distance over the audible bins
 *   - 1/3-octave band levels, centroid, 95% rolloff and flatness
 *   - harmonic levels and the non-harmonic (alias) ratio for single notes
 *   - 5 ms RMS and peak envelopes, attack and release times
 *   - global RMS, peak and a null-depth figure
 *
 * The gates are the ones in the phase-map contract; a candidate that misses
 * any of them fails. The output lists the worst scenario for each metric so a
 * failure is easy to trace. */
#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr int kN = 4096;         /* spectral frame */
constexpr int kHop = 1024;
constexpr int kEnvBlock = 240;   /* 5 ms at 48 kHz */

/* The perceptual gate. The spectral and envelope limits are the contract
 * limits; the null depth is a sanity floor, not a substitute for them. */
constexpr double kLsdMax = 0.50;
constexpr double kLsdP95 = 0.25;
constexpr double kBandMax = 0.50;
constexpr double kCentroidPct = 2.0;
constexpr double kRolloffPct = 2.0;
constexpr double kFlatness = 0.02;
constexpr double kEnvMax = 0.50;
constexpr double kEnvP95 = 0.25;
constexpr double kHarmMax = 0.50;
constexpr double kAliasDelta = 0.50;
constexpr double kAttackMs = 1.0;
constexpr double kReleaseMs = 1.0;
constexpr double kNullDb = -40.0;
constexpr double kRmsDeltaDb = 0.10;
constexpr double kPeakDeltaDb = 0.10;

struct Scenario {
    std::string id;
    int note = 0;
    int note1 = -1;
    int chord = -1;
    int wave = 0;
    int bp = 0;
    int segments = 0;
    int flip = 0, flipHz = 0;
    int reorder = 0, reorderHz = 0, jump = 0;
    int drift = 0, driftHz = 0;
    int morph = 0, morphSweep = 0;
    int lfoDepth = 0, lfoHz = 0, lfoWave = 0, lfoMode = 0;
    int envOn = 0, envMode = 0;
    int releaseAt = -1;
    int frames = 0;
    double f0 = 0.0;
};

struct Manifest {
    int mode = -1;
    int sampleRate = 0;
    int channels = 0;
    std::vector<Scenario> scenarios;
};

struct Metrics {
    double nullDb = 0.0;
    double rmsDeltaDb = 0.0;
    double peakDeltaDb = 0.0;
    double lsdMax = 0.0;
    double lsdP95 = 0.0;
    double bandMax = 0.0;
    double centroidPct = 0.0;
    double rolloffPct = 0.0;
    double flatness = 0.0;
    double envMax = 0.0;
    double envP95 = 0.0;
    double harmMax = 0.0;
    double aliasDelta = 0.0;
    double attackMs = 0.0;
    double releaseMs = 0.0;
    bool haveAttack = false;
    bool haveRelease = false;
    int frames = 0;
    bool bitExact = false;
};

std::vector<std::string> split(const std::string& line)
{
    std::vector<std::string> out;
    std::string cur;
    for (char c : line) {
        if (c == ',') {
            out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    out.push_back(cur);
    return out;
}

bool readManifest(const std::string& dir, Manifest& m)
{
    const std::string path = dir + "/manifest.csv";
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) {
        std::printf("compare: cannot read %s\n", path.c_str());
        return false;
    }
    char line[4096];
    std::vector<std::string> cols;
    while (std::fgets(line, sizeof(line), f)) {
        std::string s(line);
        while (!s.empty() && (s.back() == '\n' || s.back() == '\r'))
            s.pop_back();
        if (s.empty())
            continue;
        if (s[0] == '#') {
            int mode = -1, sr = 0, ch = 0;
            if (std::sscanf(s.c_str(),
                            "# mode=%d sample_rate=%d channels=%d", &mode,
                            &sr, &ch) == 3) {
                m.mode = mode;
                m.sampleRate = sr;
                m.channels = ch;
            }
            continue;
        }
        if (cols.empty()) {
            cols = split(s);
            continue;
        }
        const auto f = split(s);
        if (f.size() != cols.size())
            continue;
        Scenario sc;
        for (std::size_t i = 0; i < cols.size(); ++i) {
            const std::string& c = cols[i];
            const std::string& v = f[i];
            if (c == "id") sc.id = v;
            else if (c == "note") sc.note = std::stoi(v);
            else if (c == "note1") sc.note1 = std::stoi(v);
            else if (c == "chord") sc.chord = std::stoi(v);
            else if (c == "wave") sc.wave = std::stoi(v);
            else if (c == "bp") sc.bp = std::stoi(v);
            else if (c == "segments") sc.segments = std::stoi(v);
            else if (c == "flip") sc.flip = std::stoi(v);
            else if (c == "flip_hz") sc.flipHz = std::stoi(v);
            else if (c == "reorder") sc.reorder = std::stoi(v);
            else if (c == "reorder_hz") sc.reorderHz = std::stoi(v);
            else if (c == "jump") sc.jump = std::stoi(v);
            else if (c == "drift") sc.drift = std::stoi(v);
            else if (c == "drift_hz") sc.driftHz = std::stoi(v);
            else if (c == "morph") sc.morph = std::stoi(v);
            else if (c == "morph_sweep") sc.morphSweep = std::stoi(v);
            else if (c == "lfo_depth") sc.lfoDepth = std::stoi(v);
            else if (c == "lfo_hz") sc.lfoHz = std::stoi(v);
            else if (c == "lfo_wave") sc.lfoWave = std::stoi(v);
            else if (c == "lfo_mode") sc.lfoMode = std::stoi(v);
            else if (c == "env_on") sc.envOn = std::stoi(v);
            else if (c == "env_mode") sc.envMode = std::stoi(v);
            else if (c == "release_at") sc.releaseAt = std::stoi(v);
            else if (c == "frames") sc.frames = std::stoi(v);
            else if (c == "f0") sc.f0 = std::stod(v);
        }
        m.scenarios.push_back(sc);
    }
    std::fclose(f);
    return !m.scenarios.empty();
}

bool readPcm(const std::string& dir, const std::string& id,
             std::vector<int32_t>& out, int frames, int channels)
{
    const std::string path = dir + "/" + id + ".pcm";
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) {
        std::printf("compare: cannot read %s\n", path.c_str());
        return false;
    }
    out.resize(static_cast<std::size_t>(frames) * static_cast<std::size_t>(channels));
    const std::size_t got = std::fread(out.data(), sizeof(int32_t), out.size(), f);
    std::fclose(f);
    if (got != out.size()) {
        std::printf("compare: %s has %zu samples, expected %zu\n", path.c_str(),
                    got, out.size());
        return false;
    }
    return true;
}

bool compareLogs(const std::string& refDir, const std::string& candDir,
                 const char* name)
{
    const std::string a = refDir + "/" + name;
    const std::string b = candDir + "/" + name;
    std::FILE* fa = std::fopen(a.c_str(), "rb");
    std::FILE* fb = std::fopen(b.c_str(), "rb");
    if (!fa || !fb) {
        if (fa) std::fclose(fa);
        if (fb) std::fclose(fb);
        std::printf("compare: missing %s\n", name);
        return false;
    }
    char la[4096];
    char lb[4096];
    bool same = true;
    int line = 0;
    while (true) {
        char* ra = std::fgets(la, sizeof(la), fa);
        char* rb = std::fgets(lb, sizeof(lb), fb);
        ++line;
        if (!ra && !rb)
            break;
        if (!ra || !rb || std::strcmp(la, lb) != 0) {
            same = false;
            std::printf("compare: %s differs at line %d\n", name, line);
            if (ra) std::printf("  ref: %s", la);
            if (rb) std::printf("  cand: %s", lb);
            break;
        }
    }
    std::fclose(fa);
    std::fclose(fb);
    return same;
}

class Fft {
public:
    explicit Fft(int n) : n_(n), rev_(static_cast<std::size_t>(n)), tw_(static_cast<std::size_t>(n))
    {
        int bits = 0;
        while ((1 << bits) < n)
            ++bits;
        for (int i = 0; i < n; ++i) {
            int r = 0;
            for (int b = 0; b < bits; ++b)
                if (i & (1 << b))
                    r |= 1 << (bits - 1 - b);
            rev_[static_cast<std::size_t>(i)] = r;
            tw_[static_cast<std::size_t>(i)] =
                std::polar(1.0, -2.0 * kPi * static_cast<double>(i) / n);
        }
    }

    void run(std::vector<std::complex<double>>& a) const
    {
        for (int i = 0; i < n_; ++i) {
            const int j = rev_[static_cast<std::size_t>(i)];
            if (j > i)
                std::swap(a[static_cast<std::size_t>(i)], a[static_cast<std::size_t>(j)]);
        }
        for (int len = 2; len <= n_; len <<= 1) {
            const int half = len >> 1;
            const int step = n_ / len;
            for (int i = 0; i < n_; i += len) {
                for (int j = 0; j < half; ++j) {
                    const std::complex<double> w =
                        tw_[static_cast<std::size_t>(j * step)];
                    const std::complex<double> u =
                        a[static_cast<std::size_t>(i + j)];
                    const std::complex<double> v =
                        a[static_cast<std::size_t>(i + j + half)] * w;
                    a[static_cast<std::size_t>(i + j)] = u + v;
                    a[static_cast<std::size_t>(i + j + half)] = u - v;
                }
            }
        }
    }

private:
    int n_;
    std::vector<int> rev_;
    std::vector<std::complex<double>> tw_;
};

double percentile(std::vector<double> v, double q)
{
    if (v.empty())
        return 0.0;
    std::sort(v.begin(), v.end());
    const std::size_t i = static_cast<std::size_t>(
        std::floor(q * static_cast<double>(v.size() - 1)));
    return v[std::min(i, v.size() - 1)];
}

double toDb(double power, double floorPower)
{
    return 10.0 * std::log10((power + floorPower) / floorPower);
}

struct Frame {
    double centroid = 0.0;
    double rolloff = 0.0;
    double flatness = 0.0;
    std::vector<double> band; /* one-third octave powers */
};

struct Analysis {
    std::vector<double> rms;
    std::vector<double> peak;
    std::vector<Frame> frames;
    std::vector<std::vector<double>> harmonic;
    std::vector<double> alias;
    double totalPower = 0.0;
    double peakPower = 0.0;
};

/* One-third-octave centers from 25 Hz to 20 kHz. */
std::vector<double> bandCenters()
{
    std::vector<double> c;
    for (double f = 25.0; f <= 20000.0; f *= std::pow(2.0, 1.0 / 3.0))
        c.push_back(f);
    return c;
}

Analysis analyze(const std::vector<int32_t>& pcm, int frames, int channels,
                 int sr, double f0, bool harmonics)
{
    Analysis a;
    std::vector<double> mono(static_cast<std::size_t>(frames));
    for (int i = 0; i < frames; ++i) {
        double v = 0.0;
        for (int c = 0; c < channels; ++c)
            v += static_cast<double>(pcm[static_cast<std::size_t>(i) * channels + c]);
        mono[static_cast<std::size_t>(i)] = v / channels;
    }

    /* 5 ms RMS and peak envelope. */
    const int blocks = (frames + kEnvBlock - 1) / kEnvBlock;
    a.rms.resize(static_cast<std::size_t>(blocks));
    a.peak.resize(static_cast<std::size_t>(blocks));
    for (int b = 0; b < blocks; ++b) {
        const int start = b * kEnvBlock;
        const int end = std::min(frames, start + kEnvBlock);
        double sum = 0.0;
        double pk = 0.0;
        for (int i = start; i < end; ++i) {
            const double v = mono[static_cast<std::size_t>(i)];
            sum += v * v;
            pk = std::max(pk, std::abs(v));
        }
        const int n = std::max(1, end - start);
        a.rms[static_cast<std::size_t>(b)] = std::sqrt(sum / n);
        a.peak[static_cast<std::size_t>(b)] = pk;
    }

    static const Fft fft(kN);
    std::vector<std::complex<double>> buf(static_cast<std::size_t>(kN));
    std::vector<double> power(static_cast<std::size_t>(kN / 2 + 1));
    const auto centers = bandCenters();
    const bool useHarmonics = harmonics && f0 > 20.0;

    for (int start = 0; start + kN <= frames; start += kHop) {
        for (int i = 0; i < kN; ++i) {
            const double w = 0.5 - 0.5 * std::cos(2.0 * kPi * i / (kN - 1));
            buf[static_cast<std::size_t>(i)] =
                std::complex<double>(mono[static_cast<std::size_t>(start + i)] * w, 0.0);
        }
        fft.run(buf);
        double framePower = 0.0;
        for (int k = 0; k <= kN / 2; ++k) {
            const double re = buf[static_cast<std::size_t>(k)].real();
            const double im = buf[static_cast<std::size_t>(k)].imag();
            double p = re * re + im * im;
            if (k != 0 && k != kN / 2)
                p *= 2.0;
            power[static_cast<std::size_t>(k)] = p;
            if (k >= 1 && k < kN / 2)
                framePower += p;
        }
        a.totalPower += framePower;
        a.peakPower = std::max(a.peakPower, framePower);

        Frame fr;
        fr.band.assign(centers.size(), 0.0);
        double centroidNum = 0.0;
        double logSum = 0.0;
        int logCount = 0;
        const double eps = framePower / (kN / 2) * 1e-12;
        for (int k = 1; k < kN / 2; ++k) {
            const double f = static_cast<double>(k) * sr / kN;
            const double p = power[static_cast<std::size_t>(k)];
            if (f >= 20.0 && f <= 20000.0) {
                centroidNum += f * p;
                logSum += std::log(p + eps);
                ++logCount;
            }
            if (f >= 20.0 && f <= 20000.0) {
                /* The first center at or above the bin's band. */
                const double ratio = std::pow(2.0, 1.0 / 6.0);
                for (std::size_t b = 0; b < centers.size(); ++b) {
                    if (f <= centers[b] * ratio) {
                        fr.band[b] += p;
                        break;
                    }
                }
            }
        }
        fr.centroid = framePower > 0.0 ? centroidNum / framePower : 0.0;
        double acc = 0.0;
        for (int k = 1; k < kN / 2; ++k) {
            const double f = static_cast<double>(k) * sr / kN;
            acc += power[static_cast<std::size_t>(k)];
            if (acc >= 0.95 * framePower) {
                fr.rolloff = f;
                break;
            }
        }
        if (logCount > 0) {
            const double geo = std::exp(logSum / logCount);
            const double arith = framePower / logCount;
            fr.flatness = arith > 0.0 ? geo / arith : 0.0;
        }
        a.frames.push_back(std::move(fr));

        if (useHarmonics) {
            std::vector<double> h;
            double hsum = 0.0;
            for (int n = 1; n <= 64; ++n) {
                const double f = f0 * n;
                if (f >= 20000.0)
                    break;
                const int center = static_cast<int>(std::llround(f * kN / sr));
                double best = 0.0;
                for (int d = -2; d <= 2; ++d) {
                    const int k = center + d;
                    if (k < 1 || k >= kN / 2)
                        continue;
                    best = std::max(best, power[static_cast<std::size_t>(k)]);
                }
                h.push_back(best);
                hsum += best;
            }
            a.harmonic.push_back(std::move(h));
            double non = 0.0;
            for (int k = 1; k < kN / 2; ++k) {
                const double f = static_cast<double>(k) * sr / kN;
                if (f < 20.0 || f > 20000.0)
                    continue;
                const double hn = f / f0;
                const double nearest = std::round(hn);
                const bool near = nearest >= 1.0 &&
                                  std::abs(hn - nearest) <=
                                      3.0 * static_cast<double>(sr) / kN / f0;
                if (!near)
                    non += power[static_cast<std::size_t>(k)];
            }
            a.alias.push_back(10.0 * std::log10((non + eps) / (hsum + eps)));
        }
    }
    return a;
}

double rmsOf(const std::vector<int32_t>& pcm)
{
    double sum = 0.0;
    for (std::size_t i = 0; i < pcm.size(); i += 2)
        sum += static_cast<double>(pcm[i]) * pcm[i];
    return std::sqrt(sum / (pcm.size() / 2));
}

double peakOf(const std::vector<int32_t>& pcm)
{
    double pk = 0.0;
    for (std::size_t i = 0; i < pcm.size(); i += 2)
        pk = std::max(pk, std::abs(static_cast<double>(pcm[i])));
    return pk;
}

bool crossing(const std::vector<double>& env, int from, int to, double level,
              bool rising, double blockMs, double& outMs)
{
    if (from < 0) from = 0;
    if (to > static_cast<int>(env.size()) - 1) to = static_cast<int>(env.size()) - 1;
    for (int i = from; i <= to; ++i) {
        const bool above = env[static_cast<std::size_t>(i)] >= level;
        if (rising && above) {
            const double prev = i > 0 ? env[static_cast<std::size_t>(i - 1)] : 0.0;
            const double frac = level > prev && env[static_cast<std::size_t>(i)] > prev
                                    ? (level - prev) /
                                          (env[static_cast<std::size_t>(i)] - prev)
                                    : 0.0;
            outMs = (i - 1 + frac) * blockMs;
            return true;
        }
        if (!rising && !above && i > from) {
            const double prev = env[static_cast<std::size_t>(i - 1)];
            const double frac = prev > level && prev > env[static_cast<std::size_t>(i)]
                                    ? (prev - level) /
                                          (prev - env[static_cast<std::size_t>(i)])
                                    : 0.0;
            outMs = (i - 1 + frac) * blockMs;
            return true;
        }
    }
    return false;
}

Metrics compareScenario(const Scenario& s, const std::vector<int32_t>& ref,
                        const std::vector<int32_t>& cand, int sr, int channels)
{
    Metrics m;
    m.frames = s.frames;
    const bool exact =
        ref.size() == cand.size() &&
        std::memcmp(ref.data(), cand.data(), ref.size() * sizeof(int32_t)) == 0;
    m.bitExact = exact;

    const double rmsRef = rmsOf(ref);
    const double rmsCand = rmsOf(cand);
    const double peakRef = peakOf(ref);
    const double peakCand = peakOf(cand);
    m.rmsDeltaDb = 20.0 * std::log10((rmsCand + 1e-30) / (rmsRef + 1e-30));
    m.peakDeltaDb = 20.0 * std::log10((peakCand + 1e-30) / (peakRef + 1e-30));
    double diff = 0.0;
    for (std::size_t i = 0; i < ref.size(); i += 2) {
        const double d = static_cast<double>(cand[i]) - static_cast<double>(ref[i]);
        diff += d * d;
    }
    const double rmsDiff = std::sqrt(diff / (ref.size() / 2));
    m.nullDb = 20.0 * std::log10((rmsDiff + 1e-30) / (rmsRef + 1e-30));

    if (exact)
        return m;

    const bool harmonics = s.note1 < 0 && s.chord < 0 && s.f0 > 20.0;
    const Analysis ra = analyze(ref, s.frames, channels, sr, s.f0, harmonics);
    const Analysis ca = analyze(cand, s.frames, channels, sr, s.f0, harmonics);

    std::vector<double> lsd;
    std::vector<double> envDb;
    const auto centers = bandCenters();
    double bandPeak = 0.0;
    for (const Frame& rf : ra.frames)
        for (double p : rf.band)
            bandPeak = std::max(bandPeak, p);
    const double bandFloor = bandPeak * 1e-4; /* -40 dB from the loudest band */
    for (std::size_t fi = 0; fi < ra.frames.size(); ++fi) {
        const Frame& rf = ra.frames[fi];
        const Frame& cf = ca.frames[fi];

        /* Bands. The band powers are compared only where the reference band
         * carries audible energy. */
        for (std::size_t b = 0; b < centers.size(); ++b) {
            const double rp = rf.band[b];
            if (rp <= bandFloor)
                continue;
            const double cp = cf.band[b];
            const double d = 10.0 * std::log10((cp + rp * 1e-12) / (rp + rp * 1e-12));
            m.bandMax = std::max(m.bandMax, std::abs(d));
        }
        if (rf.centroid > 0.0) {
            m.centroidPct = std::max(
                m.centroidPct,
                std::abs(cf.centroid - rf.centroid) / rf.centroid * 100.0);
        }
        if (rf.rolloff > 0.0) {
            m.rolloffPct = std::max(
                m.rolloffPct,
                std::abs(cf.rolloff - rf.rolloff) / rf.rolloff * 100.0);
        }
        m.flatness = std::max(m.flatness, std::abs(cf.flatness - rf.flatness));
    }

    /* The LSD pass. Re-run the spectra with the same frame grid; the frame
     * powers are cheap compared with the FFT and this keeps the first pass
     * focused on the summary. */
    {
        static const Fft fft(kN);
        std::vector<std::complex<double>> br(static_cast<std::size_t>(kN));
        std::vector<std::complex<double>> bc(static_cast<std::size_t>(kN));
        std::vector<double> pr(static_cast<std::size_t>(kN / 2 + 1));
        std::vector<double> pc(static_cast<std::size_t>(kN / 2 + 1));
        std::vector<double> monoR(static_cast<std::size_t>(s.frames));
        std::vector<double> monoC(static_cast<std::size_t>(s.frames));
        for (int i = 0; i < s.frames; ++i) {
            double vr = 0.0, vc = 0.0;
            for (int c = 0; c < channels; ++c) {
                vr += static_cast<double>(ref[static_cast<std::size_t>(i) * channels + c]);
                vc += static_cast<double>(cand[static_cast<std::size_t>(i) * channels + c]);
            }
            monoR[static_cast<std::size_t>(i)] = vr / channels;
            monoC[static_cast<std::size_t>(i)] = vc / channels;
        }
        for (int start = 0; start + kN <= s.frames; start += kHop) {
            for (int i = 0; i < kN; ++i) {
                const double w = 0.5 - 0.5 * std::cos(2.0 * kPi * i / (kN - 1));
                br[static_cast<std::size_t>(i)] =
                    std::complex<double>(monoR[static_cast<std::size_t>(start + i)] * w, 0.0);
                bc[static_cast<std::size_t>(i)] =
                    std::complex<double>(monoC[static_cast<std::size_t>(start + i)] * w, 0.0);
            }
            fft.run(br);
            fft.run(bc);
            double framePower = 0.0;
            for (int k = 0; k <= kN / 2; ++k) {
                double rr = br[static_cast<std::size_t>(k)].real();
                double ri = br[static_cast<std::size_t>(k)].imag();
                double cr = bc[static_cast<std::size_t>(k)].real();
                double ci = bc[static_cast<std::size_t>(k)].imag();
                double rp = rr * rr + ri * ri;
                double cp = cr * cr + ci * ci;
                if (k != 0 && k != kN / 2) {
                    rp *= 2.0;
                    cp *= 2.0;
                }
                pr[static_cast<std::size_t>(k)] = rp;
                pc[static_cast<std::size_t>(k)] = cp;
                if (k >= 1 && k < kN / 2)
                    framePower += rp;
            }
            if (framePower <= 0.0)
                continue;
            double sum = 0.0;
            int count = 0;
            const double floor = framePower * 1e-5; /* -50 dB from the frame */
            for (int k = 1; k < kN / 2; ++k) {
                const double f = static_cast<double>(k) * sr / kN;
                if (f < 20.0 || f > 20000.0)
                    continue;
                const double rp = pr[static_cast<std::size_t>(k)];
                if (rp < floor)
                    continue;
                const double cp = pc[static_cast<std::size_t>(k)];
                const double d = 10.0 * std::log10((cp + rp * 1e-12) / (rp + rp * 1e-12));
                sum += d * d;
                ++count;
            }
            if (count > 0)
                lsd.push_back(std::sqrt(sum / count));
        }
    }
    m.lsdMax = lsd.empty() ? 0.0 : *std::max_element(lsd.begin(), lsd.end());
    m.lsdP95 = percentile(lsd, 0.95);

    /* Harmonic levels and alias ratio. */
    for (std::size_t fi = 0; fi < ra.harmonic.size() && fi < ca.harmonic.size(); ++fi) {
        for (std::size_t h = 0; h < ra.harmonic[fi].size(); ++h) {
            const double rp = ra.harmonic[fi][h];
            if (rp <= ra.peakPower * 1e-4)
                continue;
            const double cp = ca.harmonic[fi][h];
            const double d = 10.0 * std::log10((cp + rp * 1e-12) / (rp + rp * 1e-12));
            m.harmMax = std::max(m.harmMax, std::abs(d));
        }
        if (fi < ra.alias.size() && fi < ca.alias.size()) {
            const double d = ca.alias[fi] - ra.alias[fi];
            if (d > m.aliasDelta)
                m.aliasDelta = d;
        }
    }

    /* The envelope. */
    const double refPeak = ra.rms.empty()
                               ? 0.0
                               : *std::max_element(ra.rms.begin(), ra.rms.end());
    if (refPeak > 0.0) {
        for (std::size_t b = 0; b < ra.rms.size() && b < ca.rms.size(); ++b) {
            if (ra.rms[b] < refPeak * 1e-4)
                continue;
            const double d = 20.0 * std::log10((ca.rms[b] + 1e-30) /
                                               (ra.rms[b] + 1e-30));
            envDb.push_back(std::abs(d));
        }
        m.envMax = envDb.empty() ? 0.0 : *std::max_element(envDb.begin(), envDb.end());
        m.envP95 = percentile(envDb, 0.95);
    }

    /* Attack and release, from the 5 ms RMS envelope. */
    if (refPeak > 0.0) {
        const auto peakIt = std::max_element(ra.rms.begin(), ra.rms.end());
        const int peakIndex = static_cast<int>(peakIt - ra.rms.begin());
        const double blockMs = 1000.0 * kEnvBlock / sr;
        double a10 = 0.0, a90 = 0.0, r90 = 0.0, r10 = 0.0;
        const bool haveA10 = crossing(ra.rms, 0, peakIndex, refPeak * 0.1, true,
                                      blockMs, a10);
        const bool haveA90 = crossing(ra.rms, 0, peakIndex, refPeak * 0.9, true,
                                      blockMs, a90);
        if (haveA10 && haveA90) {
            const double refAttack = std::max(0.0, a90 - a10);
            double c10 = 0.0, c90 = 0.0;
            const bool chA10 = crossing(ca.rms, 0, peakIndex, refPeak * 0.1, true,
                                        blockMs, c10);
            const bool chA90 = crossing(ca.rms, 0, peakIndex, refPeak * 0.9, true,
                                        blockMs, c90);
            if (chA10 && chA90) {
                m.attackMs = std::abs((c90 - c10) - refAttack);
                m.haveAttack = true;
            }
        }
        const bool haveR90 = crossing(ra.rms, peakIndex,
                                      static_cast<int>(ra.rms.size()) - 1,
                                      refPeak * 0.9, false, blockMs, r90);
        const bool haveR10 = crossing(ra.rms, peakIndex,
                                      static_cast<int>(ra.rms.size()) - 1,
                                      refPeak * 0.1, false, blockMs, r10);
        if (haveR90 && haveR10) {
            const double refRelease = std::max(0.0, r10 - r90);
            double c90 = 0.0, c10 = 0.0;
            const bool chR90 = crossing(ca.rms, peakIndex,
                                        static_cast<int>(ca.rms.size()) - 1,
                                        refPeak * 0.9, false, blockMs, c90);
            const bool chR10 = crossing(ca.rms, peakIndex,
                                        static_cast<int>(ca.rms.size()) - 1,
                                        refPeak * 0.1, false, blockMs, c10);
            if (chR90 && chR10) {
                m.releaseMs = std::abs((c10 - c90) - refRelease);
                m.haveRelease = true;
            }
        }
    }
    return m;
}

bool metricFails(const Metrics& m, const Scenario& s, const char*& what,
                 double& value, double& limit)
{
    struct Gate {
        const char* name;
        double value;
        double limit;
    };
    std::vector<Gate> gates;
    gates.push_back({"null", m.nullDb, kNullDb});
    gates.push_back({"rms_delta", std::abs(m.rmsDeltaDb), kRmsDeltaDb});
    gates.push_back({"peak_delta", std::abs(m.peakDeltaDb), kPeakDeltaDb});
    gates.push_back({"lsd_max", m.lsdMax, kLsdMax});
    gates.push_back({"lsd_p95", m.lsdP95, kLsdP95});
    gates.push_back({"band_max", m.bandMax, kBandMax});
    gates.push_back({"centroid_pct", m.centroidPct, kCentroidPct});
    gates.push_back({"rolloff_pct", m.rolloffPct, kRolloffPct});
    gates.push_back({"flatness", m.flatness, kFlatness});
    gates.push_back({"env_max", m.envMax, kEnvMax});
    gates.push_back({"env_p95", m.envP95, kEnvP95});
    if (m.haveAttack)
        gates.push_back({"attack_ms", m.attackMs, kAttackMs});
    if (m.haveRelease)
        gates.push_back({"release_ms", m.releaseMs, kReleaseMs});
    if (s.note1 < 0 && s.chord < 0 && s.f0 > 20.0) {
        gates.push_back({"harm_max", m.harmMax, kHarmMax});
        gates.push_back({"alias_delta", m.aliasDelta, kAliasDelta});
    }
    for (const auto& g : gates) {
        const bool bad = g.limit < 0.0 ? (g.value > g.limit)
                                       : (std::abs(g.value) > g.limit);
        if (bad) {
            what = g.name;
            value = g.value;
            limit = g.limit;
            return true;
        }
    }
    return false;
}

double gateScore(const Metrics& m)
{
    const double nullTerm =
        m.nullDb > kNullDb ? 1.0 + (m.nullDb - kNullDb) / 20.0 : 0.0;
    return std::max({
        m.lsdMax / kLsdMax,
        m.lsdP95 / kLsdP95,
        m.bandMax / kBandMax,
        m.centroidPct / kCentroidPct,
        m.rolloffPct / kRolloffPct,
        m.flatness / kFlatness,
        m.envMax / kEnvMax,
        m.envP95 / kEnvP95,
        m.harmMax / kHarmMax,
        m.aliasDelta / kAliasDelta,
        nullTerm,
    });
}

void printScenario(const char* tag, const Scenario& s, const Metrics& m)
{
    std::printf(
        "  %-7s %-26s null %7.2f  lsd %5.3f/%5.3f  band %5.3f  "
        "cen %4.2f%%  rol %4.2f%%  flat %5.4f  env %5.3f/%5.3f  "
        "harm %5.3f  alias %5.3f  att %4.2f  rel %4.2f\n",
        tag, s.id.c_str(), m.nullDb, m.lsdMax, m.lsdP95, m.bandMax,
        m.centroidPct, m.rolloffPct, m.flatness, m.envMax, m.envP95, m.harmMax,
        m.aliasDelta, m.haveAttack ? m.attackMs : 0.0,
        m.haveRelease ? m.releaseMs : 0.0);
}

} // namespace

int main(int argc, char** argv)
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    if (argc < 3) {
        std::printf("usage: %s <reference-dir> <candidate-dir> [...]\n", argv[0]);
        return 2;
    }
    Manifest ref;
    if (!readManifest(argv[1], ref))
        return 2;
    if (ref.mode != 0)
        std::printf("compare: warning: reference mode is %d, not 0\n", ref.mode);
    std::printf("reference mode %d, %zu scenarios\n", ref.mode,
                ref.scenarios.size());

    bool allPass = true;
    for (int arg = 2; arg < argc; ++arg) {
        Manifest cand;
        if (!readManifest(argv[arg], cand)) {
            allPass = false;
            continue;
        }
        std::printf("\ncandidate mode %d (%s)\n", cand.mode, argv[arg]);
        if (cand.scenarios.size() != ref.scenarios.size()) {
            std::printf("  FAIL: scenario count %zu != %zu\n",
                        cand.scenarios.size(), ref.scenarios.size());
            allPass = false;
            continue;
        }

        const bool logs = compareLogs(argv[1], argv[arg], "geometry.csv") &&
                          compareLogs(argv[1], argv[arg], "transfer.csv") &&
                          compareLogs(argv[1], argv[arg], "qtransfer.csv");
        if (!logs) {
            std::printf("  FAIL: the map geometry or the stochastic timeline "
                        "differs from the reference\n");
            allPass = false;
        }

        std::map<std::string, size_t> index;
        for (std::size_t i = 0; i < cand.scenarios.size(); ++i)
            index[cand.scenarios[i].id] = i;

        std::vector<std::pair<double, std::size_t>> ranked;
        int failures = 0;
        bool bitExactAll = true;
        for (std::size_t i = 0; i < ref.scenarios.size(); ++i) {
            const Scenario& s = ref.scenarios[i];
            const auto it = index.find(s.id);
            if (it == index.end()) {
                std::printf("  FAIL: missing scenario %s\n", s.id.c_str());
                allPass = false;
                continue;
            }
            std::vector<int32_t> rpcm, cpcm;
            if (!readPcm(argv[1], s.id, rpcm, s.frames, ref.channels) ||
                !readPcm(argv[arg], s.id, cpcm, s.frames, cand.channels)) {
                allPass = false;
                continue;
            }
            const Metrics m = compareScenario(s, rpcm, cpcm, ref.sampleRate,
                                              ref.channels);
            if (!m.bitExact)
                bitExactAll = false;
            ranked.emplace_back(gateScore(m), i);
            const char* what = nullptr;
            double value = 0.0, limit = 0.0;
            if (!m.bitExact && metricFails(m, s, what, value, limit)) {
                ++failures;
                allPass = false;
                std::printf("  FAIL %-26s %-13s %.4f (limit %.4f)\n",
                            s.id.c_str(), what, value, limit);
            }
        }

        std::printf("  bit-exact: %s\n", bitExactAll ? "yes" : "no");
        if (cand.mode == 1 && !bitExactAll) {
            std::printf("  FAIL: the exact 4096 table is not sample-identical "
                        "to the reference\n");
            allPass = false;
        }
        if (failures == 0) {
            std::printf("  PASS: all %zu scenarios inside the gate\n",
                        ref.scenarios.size());
        } else {
            std::printf("  %d scenario(s) outside the gate\n", failures);
        }

        std::sort(ranked.begin(), ranked.end(),
                  [](const auto& a, const auto& b) { return a.first > b.first; });
        std::printf("  worst scenarios by gate score:\n");
        const std::size_t show = std::min<std::size_t>(10, ranked.size());
        for (std::size_t k = 0; k < show; ++k) {
            const Scenario& s = ref.scenarios[ranked[k].second];
            std::vector<int32_t> rpcm, cpcm;
            if (!readPcm(argv[1], s.id, rpcm, s.frames, ref.channels) ||
                !readPcm(argv[arg], s.id, cpcm, s.frames, cand.channels))
                continue;
            const Metrics m = compareScenario(s, rpcm, cpcm, ref.sampleRate,
                                              ref.channels);
            printScenario("worst", s, m);
        }
    }
    std::printf("\n%s\n", allPass ? "perceptual: PASS" : "perceptual: FAIL");
    return allPass ? 0 : 1;
}
