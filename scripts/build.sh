#!/bin/bash

set -euo pipefail

echo "Building PHOENIX..."

cd ~/ns-3-dev

./ns3 build

echo "Build Complete."