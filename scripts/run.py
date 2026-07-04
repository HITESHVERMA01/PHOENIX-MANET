"""
PHOENIX Run Tool
"""

import subprocess
from pathlib import Path

NS3_PATH = Path(r"D:\ns-allinone-3.36\ns-3.36")

subprocess.run(
    ["./ns3", "run", "scratch/logger-test"],
    cwd=NS3_PATH,
    check=True,
)