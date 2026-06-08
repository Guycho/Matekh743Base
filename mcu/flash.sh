#!/bin/bash
# -----------------------------------------------------------------------------
# flash.sh — Build and Flash via DFU for Matek H743-Mini
# -----------------------------------------------------------------------------
set -eu

# Get the directory where the script is located
ROOT="$(cd "$(dirname "$0")" && pwd)"

TARGET="matek743mini"
JOBS="${JOBS:-$(nproc)}"
MCU_DEBUG_LOG=1
DO_BUILD_ONLY=0
SHOULD_CLEAN=1

while [ "$#" -gt 0 ]; do
    case "$1" in
        undebug)
            MCU_DEBUG_LOG=0
            shift
            ;;
        build)
            DO_BUILD_ONLY=1
            shift
            ;;
        --not-clean)
            SHOULD_CLEAN=0
            shift
            ;;
        --jobs)
            if [ "$#" -lt 2 ]; then
                echo "Error: --jobs requires a value"
                exit 1
            fi
            JOBS="$2"
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [debug] [build] [--clean] [--jobs N]"
            exit 0
            ;;
        *)
            echo "Error: Unknown argument: $1"
            echo "Usage: $0 [debug] [build] [--clean] [--jobs N]"
            exit 1
            ;;
    esac
done

cd "$ROOT"

if [ "$MCU_DEBUG_LOG" = "1" ]; then
    BUILD_DIR="${BUILD_DIR:-build/debug}"
    echo "=== Configuration: debug printing enabled ==="
else
    BUILD_DIR="${BUILD_DIR:-build/release}"
    echo "=== Configuration: debug printing disabled ==="
fi

if [ "$SHOULD_CLEAN" -eq 1 ]; then
    echo "=== Cleaning Project ==="
    make clean BUILD_DIR="$BUILD_DIR"
    echo ""
fi

echo "=== Building Project ==="
make -j"$JOBS" BUILD_DIR="$BUILD_DIR" MCU_DEBUG_LOG="$MCU_DEBUG_LOG"

# 2. Find the generated binary
BIN_FILE="${BUILD_DIR}/${TARGET}.bin"

if [ ! -f "$BIN_FILE" ]; then
    echo "Error: Binary file $BIN_FILE not found. Build failed."
    exit 1
fi

if [ "$DO_BUILD_ONLY" -eq 1 ]; then
    echo ""
    echo "=== Build OK: $BIN_FILE (skipping flash) ==="
    exit 0
fi

# 3. Flash via DFU
if ! command -v dfu-util >/dev/null 2>&1; then
    echo "Error: dfu-util not found in PATH. Please install it to flash the board."
    exit 1
fi

echo ""
echo "=== Flashing $BIN_FILE via DFU ==="
# -a 0: target the internal flash
# -s 0x08000000: leave the device after flashing from the start of flash memory
# -D: download file
DFU_LOG="$(mktemp)"
set +e
dfu-util -d 0483:df11 -a 0 -s 0x08000000:leave -D "$BIN_FILE" 2>&1 | tee "$DFU_LOG"
DFU_STATUS="${PIPESTATUS[0]}"
set -e
if [ "$DFU_STATUS" -ne 0 ]; then
    if grep -q "File downloaded successfully" "$DFU_LOG" && grep -q "Error during download get_status" "$DFU_LOG"; then
        echo "Ignoring dfu-util post-leave status error after a successful download."
    else
        rm -f "$DFU_LOG"
        exit "$DFU_STATUS"
    fi
fi
rm -f "$DFU_LOG"

echo ""
echo "=== Done! ==="