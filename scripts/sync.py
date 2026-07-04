"""
============================================================
PHOENIX Sync Tool
Version : v0.1
============================================================
Copies PHOENIX source files into the NS-3 source tree.
"""

import shutil
from pathlib import Path

# -------- CHANGE THIS PATH ON YOUR COMPUTER --------
NS3_PATH = Path(r"D:\ns-allinone-3.36\ns-3.36")
# ---------------------------------------------------

PHOENIX_SRC = Path(__file__).parent.parent / "src" / "phoenix"
NS3_DST = NS3_PATH / "src" / "phoenix"

print("=" * 60)
print("PHOENIX Sync Tool")
print("=" * 60)

if not NS3_PATH.exists():
    print(f"❌ NS-3 not found at: {NS3_PATH}")
    print("Please update NS3_PATH in sync.py")
    raise SystemExit(1)

NS3_DST.mkdir(parents=True, exist_ok=True)

copied = 0

for file in PHOENIX_SRC.glob("*"):
    if file.is_file():
        shutil.copy2(file, NS3_DST / file.name)
        copied += 1
        print(f"✓ Copied {file.name}")

print("-" * 60)
print(f"Total files copied: {copied}")
print("Sync complete.")