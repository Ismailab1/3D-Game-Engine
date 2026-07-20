#!/usr/bin/env bash
# build.sh — configure and build the 3D Game Engine on Linux / macOS.
# Usage: ./scripts/build.sh [Debug|Release]
set -euo pipefail

BUILD_TYPE="${1:-Debug}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$SRC_DIR/out/build/linux-$BUILD_TYPE"

echo ""
echo "============================================================"
echo " 3D Game Engine | Build Script  ($BUILD_TYPE)"
echo "============================================================"
echo "[build] Source : $SRC_DIR"
echo "[build] Output : $BUILD_DIR"
echo ""

# ── Verify cmake ─────────────────────────────────────────────────────────────
if ! command -v cmake &>/dev/null; then
    echo "[ERROR] cmake not found."
    echo "        sudo apt-get install cmake   |  brew install cmake"
    exit 1
fi

# ── Prefer Ninja if available ─────────────────────────────────────────────────
GENERATOR_ARGS=()
if command -v ninja &>/dev/null; then
    GENERATOR_ARGS=(-G Ninja)
fi

# ── Configure (only when no cache exists) ────────────────────────────────────
if [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
    echo "[build] Configuring ..."
    cmake -S "$SRC_DIR" -B "$BUILD_DIR" \
        "${GENERATOR_ARGS[@]}" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
fi

# ── Build ─────────────────────────────────────────────────────────────────────
echo "[build] Building ..."
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" --parallel

echo ""
echo "[build] SUCCESS | $BUILD_DIR"
