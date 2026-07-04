"""
PHOENIX Build Tool
"""

import subprocess
from pathlib import Path

NS3_PATH = Path(r"D:\ns-allinone-3.36\ns-3.36")

print("Building NS-3...")

subprocess.run(
    ["./ns3", "build"],
    cwd=NS3_PATH,
    check=True,
)
print("Build completed.")