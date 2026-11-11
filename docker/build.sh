#!/bin/sh
# Build and test Type P.alpha for a Linux platform with clang and lld.
#
#   docker/build.sh linux/amd64
#   docker/build.sh linux/arm64
#   docker/build.sh linux/amd64 address,undefined
#
# linux/arm64 needs QEMU/binfmt on an x86_64 host, or a native arm64 host. On
# Docker Desktop, the emulation setting provides it.
set -eu
cd "$(dirname "$0")/.."

platform="${1:-linux/amd64}"
sanitizers="${2:-}"
name=$(printf '%s' "$platform" | tr '/' '-')
image="type-p-alpha-build:${name}"
build_dir="build/${name}-clang"

cmake_args="-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld \
    -DCMAKE_SHARED_LINKER_FLAGS=-fuse-ld=lld \
    -DCMAKE_MODULE_LINKER_FLAGS=-fuse-ld=lld"

if [ -n "$sanitizers" ]; then
    build_dir="${build_dir}-sanitize"
    cmake_args="$cmake_args -DP1_SANITIZERS=$sanitizers"
fi

docker buildx build --platform "$platform" --load -t "$image" \
    -f docker/Dockerfile .
docker run --rm --platform "$platform" -v "$PWD:/work" -w /work "$image" \
    sh -c "cmake -S . -B '$build_dir' -G Ninja -DCMAKE_BUILD_TYPE=Release $cmake_args && \
           cmake --build '$build_dir' && \
           ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 \
           ctest --test-dir '$build_dir' \
             -E 'linux_daws|plugin_editor_snapshot|plugin_editor_snapshot_diff' \
             --output-on-failure"
