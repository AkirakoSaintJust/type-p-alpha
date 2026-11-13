/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>

#include <juce_core/juce_core.h>

namespace p1::params {

inline juce::String partSuffix(int part)
{
    return ".p" + juce::String(part);
}

inline juce::String withPart(const juce::String& base, int part)
{
    return base + partSuffix(part);
}

inline juce::String algorithm(int part) { return withPart("algorithm", part); }
inline juce::String channel(int part) { return withPart("channel", part); }
inline juce::String keyLow(int part) { return withPart("low", part); }
inline juce::String keyHigh(int part) { return withPart("high", part); }
inline juce::String level(int part) { return withPart("level", part); }
inline juce::String delay(int part) { return withPart("delay", part); }
inline juce::String detune(int part) { return withPart("detune", part); }
inline juce::String transpose(int part) { return withPart("transpose", part); }
inline juce::String mode(int part) { return withPart("mode", part); }
inline juce::String legato(int part) { return withPart("legato", part); }
inline juce::String unisonCount(int part) { return withPart("unison_count", part); }
inline juce::String unisonDetune(int part) { return withPart("unison_detune", part); }
inline juce::String portamento(int part) { return withPart("portamento", part); }
inline juce::String notePriority(int part) { return withPart("note_priority", part); }

inline juce::String opBase(int op, const char* field)
{
    return "op" + juce::String(op + 1) + field;
}

inline juce::String opRatio(int op, int part)
{
    return withPart(opBase(op, "_ratio"), part);
}
inline juce::String opDetune(int op, int part)
{
    return withPart(opBase(op, "_detune"), part);
}
inline juce::String opWaveform(int op, int part)
{
    return withPart(opBase(op, "_waveform"), part);
}
inline juce::String opLevel(int op, int part)
{
    return withPart(opBase(op, "_level"), part);
}
inline juce::String opFeedback(int op, int part)
{
    return withPart(opBase(op, "_feedback"), part);
}
inline juce::String opFeedbackOn(int op, int part)
{
    return withPart(opBase(op, "_fbenv_on"), part);
}
inline juce::String opFeedbackLfoRate(int op, int part)
{
    return withPart(opBase(op, "_fblfo_rate"), part);
}
inline juce::String opFeedbackLfoDepth(int op, int part)
{
    return withPart(opBase(op, "_fblfo_depth"), part);
}
inline juce::String opFeedbackLfoWave(int op, int part)
{
    return withPart(opBase(op, "_fblfo_wave"), part);
}
inline juce::String opFeedbackLfoMode(int op, int part)
{
    return withPart(opBase(op, "_fblfo_mode"), part);
}
inline juce::String opDetuneLfoRate(int op, int part)
{
    return withPart(opBase(op, "_dlfo_rate"), part);
}
inline juce::String opDetuneLfoDepth(int op, int part)
{
    return withPart(opBase(op, "_dlfo_depth"), part);
}
inline juce::String opDetuneLfoWave(int op, int part)
{
    return withPart(opBase(op, "_dlfo_wave"), part);
}
inline juce::String opDetuneLfoMode(int op, int part)
{
    return withPart(opBase(op, "_dlfo_mode"), part);
}
inline juce::String opTremRate(int op, int part)
{
    return withPart(opBase(op, "_trem_rate"), part);
}
inline juce::String opTremDepth(int op, int part)
{
    return withPart(opBase(op, "_trem_depth"), part);
}
inline juce::String opTremWave(int op, int part)
{
    return withPart(opBase(op, "_trem_wave"), part);
}
inline juce::String opTremMode(int op, int part)
{
    return withPart(opBase(op, "_trem_mode"), part);
}
inline juce::String opMute(int op, int part)
{
    return withPart(opBase(op, "_mute"), part);
}

inline juce::String opEnvRate(int op, int index, int part)
{
    if (op == 0 && index == 0)
        return withPart("attack", part);
    if (op == 0 && index == 3)
        return withPart("release", part);
    return "op" + juce::String(op + 1) + ".eg.r" + juce::String(index + 1) +
           partSuffix(part);
}

inline juce::String opEnvLevel(int op, int index, int part)
{
    return "op" + juce::String(op + 1) + ".eg.l" + juce::String(index + 1) +
           partSuffix(part);
}

inline juce::String opFeedbackRate(int op, int index, int part)
{
    return "op" + juce::String(op + 1) + "_fbenv_r" +
           juce::String(index + 1) + partSuffix(part);
}
inline juce::String opFeedbackLevel(int op, int index, int part)
{
    return "op" + juce::String(op + 1) + "_fbenv_l" +
           juce::String(index + 1) + partSuffix(part);
}

inline juce::String pitchRate(int part) { return withPart("pitch_rate", part); }
inline juce::String pitchDepth(int part) { return withPart("pitch_depth", part); }
inline juce::String pitchWave(int part) { return withPart("pitch_wave", part); }
inline juce::String pitchMode(int part) { return withPart("pitch_mode", part); }
inline juce::String pitchEnvOn(int part) { return withPart("penv_on", part); }
inline juce::String pitchEnvRate(int index, int part)
{
    return withPart("penv_r" + juce::String(index + 1), part);
}
inline juce::String pitchEnvLevel(int index, int part)
{
    return withPart("penv_l" + juce::String(index + 1), part);
}

inline juce::String filterCutoff(int part) { return withPart("cutoff", part); }
inline juce::String filterRes(int part) { return withPart("res", part); }
inline juce::String filterType(int part) { return withPart("filter_type", part); }
inline juce::String filterOn(int part) { return withPart("filter_on", part); }
inline juce::String filterEnvDepth(int part) { return withPart("fenv_depth", part); }
inline juce::String filterRate(int index, int part)
{
    return withPart("fenv_r" + juce::String(index + 1), part);
}
inline juce::String filterLevel(int index, int part)
{
    return withPart("fenv_l" + juce::String(index + 1), part);
}

struct FollowTarget {
    const char* name;
    juce::String keyLo;
    juce::String keyHi;
    juce::String velocity;
};

inline std::array<FollowTarget, 15> followTargets(int part)
{
    return {{
        {"op1 level", withPart("op1_level_ks_lo", part),
         withPart("op1_level_ks_hi", part), withPart("op1_level_vs", part)},
        {"op1 feedback", withPart("op1_fb_ks_lo", part),
         withPart("op1_fb_ks_hi", part), withPart("op1_fb_vs", part)},
        {"op1 amp env", withPart("op1_aenv_ks_lo", part),
         withPart("op1_aenv_ks_hi", part), withPart("op1_aenv_vs", part)},
        {"op2 level", withPart("op2_level_ks_lo", part),
         withPart("op2_level_ks_hi", part), withPart("op2_level_vs", part)},
        {"op2 feedback", withPart("op2_fb_ks_lo", part),
         withPart("op2_fb_ks_hi", part), withPart("op2_fb_vs", part)},
        {"op2 amp env", withPart("op2_aenv_ks_lo", part),
         withPart("op2_aenv_ks_hi", part), withPart("op2_aenv_vs", part)},
        {"op3 level", withPart("op3_level_ks_lo", part),
         withPart("op3_level_ks_hi", part), withPart("op3_level_vs", part)},
        {"op3 feedback", withPart("op3_fb_ks_lo", part),
         withPart("op3_fb_ks_hi", part), withPart("op3_fb_vs", part)},
        {"op3 amp env", withPart("op3_aenv_ks_lo", part),
         withPart("op3_aenv_ks_hi", part), withPart("op3_aenv_vs", part)},
        {"op4 level", withPart("op4_level_ks_lo", part),
         withPart("op4_level_ks_hi", part), withPart("op4_level_vs", part)},
        {"op4 feedback", withPart("op4_fb_ks_lo", part),
         withPart("op4_fb_ks_hi", part), withPart("op4_fb_vs", part)},
        {"op4 amp env", withPart("op4_aenv_ks_lo", part),
         withPart("op4_aenv_ks_hi", part), withPart("op4_aenv_vs", part)},
        {"pitch env", withPart("penv_ks_lo", part),
         withPart("penv_ks_hi", part), withPart("penv_vs", part)},
        {"filter 1 cutoff", withPart("f1_cutoff_ks_lo", part),
         withPart("f1_cutoff_ks_hi", part), withPart("f1_cutoff_vs", part)},
        {"filter env", withPart("fenv_ks_lo", part), withPart("fenv_ks_hi", part),
         withPart("fenv_vs", part)},
    }};
}

inline juce::StringArray waveNames()
{
    return {"sine", "triangle", "half sine", "half tri", "2x sine", "2x tri",
            "2x half sine", "2x half tri", "rect sine", "rect tri", "pulse",
            "square", "saw"};
}

inline juce::StringArray lfoWaveNames()
{
    return {"sine", "triangle", "saw", "inverse saw", "square", "random"};
}

} // namespace p1::params
