#!/bin/bash

set -euo pipefail

echo "Running PHOENIX..."

cd ~/ns-3-dev

if [ "$#" -eq 0 ]; then
    ./ns3 run scratch/phoenix
else
    ./ns3 run "scratch/phoenix $*"
fi

echo "Simulation Finished."