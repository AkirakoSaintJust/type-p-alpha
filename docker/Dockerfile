# Type P.alpha Linux build image. It is multi-arch: build it for linux/amd64 or
# linux/arm64 with buildx. It carries the clang toolchain, clang-tidy and
# clang-format, the JUCE build dependencies and Xvfb for the editor snapshots,
# not the DAW acceptance hosts. The Linux DAW test uses a host with Ardour, Carla
# and JACK, or the CI runner.

FROM debian:trixie-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential clang lld libclang-rt-dev clang-tidy clang-format \
        cmake ninja-build git python3 pkg-config zip ca-certificates \
        xvfb xauth libgl1-mesa-dri \
        libasound2-dev libfreetype-dev libfontconfig1-dev \
        libgl1-mesa-dev libegl-dev libgles2 libxi-dev \
        libx11-dev libxext-dev libxrandr-dev \
        libxinerama-dev libxcursor-dev libxcomposite-dev libxrender-dev \
        libxfixes-dev libxkbcommon-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /work
CMD ["/bin/bash"]
