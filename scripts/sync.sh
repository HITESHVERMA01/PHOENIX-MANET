#!/bin/bash

set -euo pipefail

echo "Syncing PHOENIX to NS-3..."

SOURCE="/mnt/d/projects/PHOENIX-MANET/scratch/phoenix.cc"
TARGET="$HOME/ns-3-dev/scratch/phoenix.cc"

if [ "$(realpath "$SOURCE")" = "$(realpath "$TARGET")" ]; then
    echo "PHOENIX source is already linked to NS-3; no copy needed."
else
    cp "$SOURCE" "$TARGET"
    echo "Source copied to NS-3."
fi

echo "Done."