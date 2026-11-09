#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
# Linux DAW acceptance: Ardour renders the VST3, Carla renders both formats.
#
# Usage: linux_daws.sh <vst3-bundle> [work-dir] [clap-binary]
set -eu

VST3=${1:?usage: linux_daws.sh <vst3-bundle> [work-dir] [clap-binary]}
WORK=${2:-"$(pwd)/linux-daws"}
CLAP=${3:-}

if [ ! -d "$VST3" ]; then
    echo "linux-daws: VST3 bundle not found: $VST3" >&2
    exit 1
fi
if [ -n "$CLAP" ] && [ ! -f "$CLAP" ]; then
    echo "linux-daws: CLAP binary not found: $CLAP" >&2
    exit 1
fi
case "$WORK" in
    /|"")
        echo "linux-daws: refusing unsafe work directory: $WORK" >&2
        exit 1
        ;;
esac

for tool in ardour8-new_session ardour8-lua ardour8-export \
            carla-jack-single jackd jack_lsp jack_rec oscsend \
            xvfb-run python3 timeout readlink; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "linux-daws: missing $tool" >&2
        exit 1
    fi
done

rm -rf "$WORK"
mkdir -p "$WORK/home/.vst3" "$WORK/home/.clap" "$WORK/home/.config" \
         "$WORK/home/.local/share" "$WORK/ardour"
cp -a "$VST3" "$WORK/home/.vst3/"
if [ -n "$CLAP" ]; then
    cp -a "$CLAP" "$WORK/home/.clap/"
fi

export HOME="$WORK/home"
export XDG_CONFIG_HOME="$HOME/.config"
export XDG_DATA_HOME="$HOME/.local/share"
export LANG=C.UTF-8

ARDOUR_UTIL="$(readlink -f "$(command -v ardour8-export)")"
ARDOUR_LIB_DIR="$(cd "$(dirname "$ARDOUR_UTIL")/.." && pwd)"
VST3_SCANNER="$ARDOUR_LIB_DIR/ardour-vst3-scanner"
if [ ! -x "$VST3_SCANNER" ]; then
    echo "linux-daws: missing $VST3_SCANNER" >&2
    exit 1
fi
scan_vst3() {
    LD_LIBRARY_PATH="$ARDOUR_LIB_DIR" "$VST3_SCANNER" \
        "$WORK/home/.vst3/$(basename "$VST3")" >"$WORK/ardour-scan.log" 2>&1
}

if ! scan_vst3; then
    echo "linux-daws: the VST3 scanner failed" >&2
    cat "$WORK/ardour-scan.log" >&2
    exit 1
fi
if ! ls "$HOME/.cache/ardour8/vst/"*.v3i >/dev/null 2>&1; then
    echo "linux-daws: the VST3 scanner wrote no cache entry" >&2
    cat "$WORK/ardour-scan.log" >&2
    exit 1
fi

check_wav() {
    python3 - "$1" "$2" <<'PY'
import math
import struct
import sys
import wave

path = sys.argv[1]
label = sys.argv[2]
with wave.open(path, "rb") as wav:
    frames = wav.getnframes()
    rate = wav.getframerate()
    channels = wav.getnchannels()
    width = wav.getsampwidth()
    data = wav.readframes(frames)

if width == 3:
    values = []
    for i in range(0, len(data), 3):
        value = data[i] | (data[i + 1] << 8) | (data[i + 2] << 16)
        if value & 0x800000:
            value -= 1 << 24
        values.append(value)
    scale = 8388608.0
elif width == 2:
    values = struct.unpack("<%dh" % (len(data) // 2), data)
    scale = 32768.0
else:
    raise SystemExit("unexpected WAV sample width %d" % width)

peak = max(abs(value) for value in values) / scale
rms = math.sqrt(sum((value / scale) ** 2 for value in values) / len(values))
print("%s: frames=%d rate=%d channels=%d peak=%.6f rms=%.6f"
      % (label, frames, rate, channels, peak, rms))
if frames == 0 or peak <= 0.001:
    raise SystemExit(label + " rendered silence")
PY
}

SESSION_DIR="$WORK/ardour/session"
SESSION_NAME="TypePFm1"
rm -rf "$SESSION_DIR"
ardour8-new_session -s 44100 "$SESSION_DIR" "$SESSION_NAME" \
    >"$WORK/ardour-new.log" 2>&1 || true
if [ ! -f "$SESSION_DIR/$SESSION_NAME.ardour" ]; then
    echo "linux-daws: Ardour did not create a session" >&2
    tail -20 "$WORK/ardour-new.log" >&2
    exit 1
fi

cat >"$WORK/add_track.lua" <<LUA
backend = AudioEngine:set_backend('None (Dummy)', '', '')
local f = io.open('$WORK/add_track.out', 'w')
local s = load_session('$SESSION_DIR', '$SESSION_NAME')
assert(s)
local info
for p in ARDOUR.LuaAPI.list_plugins():iter() do
  if p.name == 'Type P.alpha' then info = p end
end
f:write('plugin=' .. tostring(info) .. '\n')
assert(info, 'Type P.alpha not found')
local list = s:new_midi_track(
  ARDOUR.ChanCount(ARDOUR.DataType('midi'), 1),
  ARDOUR.ChanCount(ARDOUR.DataType('audio'), 2),
  true, info, nil, nil, 1, 'Type P.alpha',
  ARDOUR.PresentationInfo.max_order, ARDOUR.TrackMode.Normal, true, false)
assert(list and not list:empty())
f:write('track=' .. list:front():name() .. '\n')
s:save_state('')
f:close()
LUA

ardour_track() {
    rm -f "$WORK/add_track.out"
    timeout 90s xvfb-run -a ardour8-lua "$WORK/add_track.lua" \
        >"$WORK/ardour-lua.log" 2>&1 || true
    grep -q 'type="vst3"' "$SESSION_DIR/$SESSION_NAME.ardour"
}

if ! ardour_track; then
    # Ardour can miss a freshly written VST3 cache on a loaded host. Scan
    # again and retry once, so a cold cache does not fail the gate. A plugin
    # that fails twice still fails, and the scan log explains why.
    scan_vst3 || true
    if ! ardour_track; then
        echo "linux-daws: Ardour did not save the VST3 track" >&2
        cat "$WORK/add_track.out" 2>/dev/null >&2 || true
        echo "linux-daws: VST3 scan log:" >&2
        cat "$WORK/ardour-scan.log" >&2 || true
        echo "linux-daws: plugin cache:" >&2
        ls -l "$HOME/.cache/ardour8/vst/" >&2 || true
        tail -20 "$WORK/ardour-lua.log" >&2
        exit 1
    fi
fi

python3 - "$SESSION_DIR" "$SESSION_NAME" <<'PY'
import struct
import sys
from pathlib import Path

session_dir = Path(sys.argv[1])
session_name = sys.argv[2]
base = session_dir
midi_dir = base / "interchange" / session_dir.name / "midifiles"
midi_dir.mkdir(parents=True, exist_ok=True)


def vlq(n):
    out = [n & 0x7F]
    n >>= 7
    while n:
        out.append((n & 0x7F) | 0x80)
        n >>= 7
    return bytes(reversed(out))


track = b""
track += vlq(0) + bytes([0x90, 60, 100])
track += vlq(1920) + bytes([0x80, 60, 0])
track += vlq(0) + bytes([0xFF, 0x2F, 0x00])
data = b"MThd" + struct.pack(">IHHH", 6, 0, 1, 960)
data += b"MTrk" + struct.pack(">I", len(track)) + track
(midi_dir / "Type P.alpha.mid").write_bytes(data)

path = base / (session_name + ".ardour")
text = path.read_text()
sources = (
    "<Sources>\n"
    '    <Source name="Type P.alpha.mid" type="midi" '
    'flags="Writable,CanRename" id="500" origin=""/>\n'
    "  </Sources>"
)
locations = (
    "<Locations>\n"
    '    <Location id="600" name="session" start="0" end="88200" '
    'flags="IsSessionRange" locked="no" position-lock-style="AudioTime"/>\n'
    '    <Location id="601" name="start" start="0" end="0" '
    'flags="IsMark,IsStart" locked="no" position-lock-style="AudioTime"/>\n'
    '    <Location id="602" name="end" start="88200" end="88200" '
    'flags="IsMark,IsEnd" locked="no" position-lock-style="AudioTime"/>\n'
    "  </Locations>"
)
text = text.replace("<Sources/>", sources)
text = text.replace("<Locations/>", locations)

region = (
    '      <Region locked="0" name="Type P.alpha" position="0" length="88200" '
    'stretch="1" muted="0" opaque="1" automatic="0" whole-file="0" '
    'import="0" external="0" sync-marked="0" left-of-split="0" '
    'right-of-split="0" hidden="0" position-locked="0" valid-transients="0" '
    'start="0" sync-position="0" ancestral-start="0" ancestral-length="0" '
    'shift="1" positional-lock-style="MusicTime" layering-index="0" '
    'start-beats="0" length-beats="4" id="501" type="midi" '
    'first-edit="nothing" bbt-position="1|1|0" source-0="500" '
    'master-source-0="500"/>\n'
)
marker = '<Playlist'
start = text.find(marker)
while start != -1:
    end = text.find("/>", start)
    if end == -1:
        break
    tag = text[start:end + 2]
    if 'type="midi"' in tag:
        opened = tag[:-2].rstrip() + ">\n"
        replacement = opened + region + "    </Playlist>"
        text = text[:start] + replacement + text[end + 2:]
        break
    start = text.find(marker, end + 2)
else:
    raise SystemExit("MIDI playlist not found")

path.write_text(text)
print("ardour session patched")
PY

timeout 120s ardour8-export -b 24 -s 44100 -o "$WORK/ardour.wav" \
    "$SESSION_DIR" "$SESSION_NAME" >"$WORK/ardour-export.log" 2>&1
if [ ! -f "$WORK/ardour.wav" ]; then
    echo "linux-daws: Ardour did not export audio" >&2
    tail -30 "$WORK/ardour-export.log" >&2
    exit 1
fi

check_wav "$WORK/ardour.wav" ardour

CARLA_CONFIG="$HOME/.config/falkTX"
mkdir -p "$CARLA_CONFIG"
cat >"$CARLA_CONFIG/Carla2.conf" <<'INI'
[OSC]
Enabled=true
TCPEnabled=true
TCPNumber=12345
TCPRandom=false
UDPEnabled=true
UDPNumber=12345
UDPRandom=false
INI

CARXP="$WORK/empty.carxp"
cat >"$CARXP" <<'XML'
<?xml version='1.0' encoding='UTF-8'?>
<!DOCTYPE CARLA-PROJECT>
<CARLA-PROJECT VERSION='2.0'>
 <EngineSettings>
  <ForceStereo>false</ForceStereo>
  <PreferPluginBridges>false</PreferPluginBridges>
  <PreferUiBridges>false</PreferUiBridges>
  <UIsAlwaysOnTop>false</UIsAlwaysOnTop>
  <MaxParameters>200</MaxParameters>
  <UIBridgesTimeout>4000</UIBridgesTimeout>
 </EngineSettings>
</CARLA-PROJECT>
XML

JACK_PID=""
CARLA_PID=""
cleanup() {
    if [ -n "$CARLA_PID" ]; then
        kill "$CARLA_PID" 2>/dev/null || true
        wait "$CARLA_PID" 2>/dev/null || true
    fi
    if [ -n "$JACK_PID" ]; then
        kill "$JACK_PID" 2>/dev/null || true
        wait "$JACK_PID" 2>/dev/null || true
    fi
}
trap cleanup EXIT HUP INT TERM

if ! jack_lsp >/dev/null 2>&1; then
    jackd --no-realtime -d dummy -r 44100 -p 256 >"$WORK/jackd.log" 2>&1 &
    JACK_PID=$!
    for _ in $(seq 1 50); do
        jack_lsp >/dev/null 2>&1 && break
        sleep 0.1
    done
fi
if ! jack_lsp >/dev/null 2>&1; then
    echo "linux-daws: JACK did not start" >&2
    echo "linux-daws: the container needs --shm-size=512m" >&2
    tail -20 "$WORK/jackd.log" >&2
    exit 1
fi

carla_start() {
    if [ -n "$CARLA_PID" ]; then
        kill "$CARLA_PID" 2>/dev/null || true
        wait "$CARLA_PID" 2>/dev/null || true
        CARLA_PID=""
        sleep 1
    fi
    QT_QPA_PLATFORM=offscreen carla-jack-single --no-gui "$CARXP" \
        >"$WORK/carla.log" 2>&1 &
    CARLA_PID=$!
    sleep 2
    if ! kill -0 "$CARLA_PID" 2>/dev/null; then
        echo "linux-daws: Carla did not start" >&2
        tail -30 "$WORK/carla.log" >&2
        exit 1
    fi
}

carla_add() {
    python3 - "$1" "$2" "$3" <<'PY'
import sys
import time

import pyliblo3 as liblo

filename, label, ptype = sys.argv[1], sys.argv[2], int(sys.argv[3])
server = liblo.Server(0, proto="TCP")
target = liblo.Address("osc.tcp://127.0.0.1:12345")
liblo.send(target, "/register", server.get_url() + "Carla")
time.sleep(0.3)
liblo.send(
    target,
    "/ctrl/add_plugin",
    3,
    2,
    ptype,
    filename,
    "Type P.alpha",
    label,
    0,
    0,
)
for _ in range(30):
    server.recv(100)
PY
}

carla_record() {
    tag=$1
    filename=$2
    label=$3
    ptype=$4
    carla_start
    carla_add "$filename" "$label" "$ptype"
    for _ in $(seq 1 50); do
        if jack_lsp 2>/dev/null | grep -q 'Carla:Type P.alpha:output_1'; then
            break
        fi
        sleep 0.1
    done
    if ! jack_lsp 2>/dev/null | grep -q 'Carla:Type P.alpha:output_1'; then
        echo "linux-daws: Carla did not load Type P.alpha as $tag" >&2
        tail -30 "$WORK/carla.log" >&2
        exit 1
    fi

    wav="$WORK/carla-$tag.wav"
    rm -f "$wav"
    # The dummy JACK backend reports overruns on a loaded host even when the
    # audio is intact. A larger disk buffer keeps the take instead of writing
    # a silent WAV.
    jack_rec -f "$wav" -d 3 -b 24 -B 65536 \
        'Carla:Type P.alpha:output_1' 'Carla:Type P.alpha:output_2' \
        >"$WORK/jackrec-$tag.log" 2>&1 &
    record_pid=$!
    sleep 0.5
    oscsend osc.udp://127.0.0.1:12345 /Carla/0/note_on iii 0 60 100
    sleep 1
    oscsend osc.udp://127.0.0.1:12345 /Carla/0/note_off ii 0 60
    wait "$record_pid" || true
    if [ ! -f "$wav" ]; then
        echo "linux-daws: Carla did not record $tag audio" >&2
        tail -20 "$WORK/jackrec-$tag.log" >&2
        exit 1
    fi
    check_wav "$wav" "carla-$tag"
}

carla_record vst3 "$WORK/home/.vst3/Type P.alpha.vst3" "Type P.alpha" 6
if [ -n "$CLAP" ]; then
    carla_record clap "$WORK/home/.clap/Type P.alpha.clap" \
        "com.p1onken.typepalpha" 14
fi

echo "linux-daws: ok"
