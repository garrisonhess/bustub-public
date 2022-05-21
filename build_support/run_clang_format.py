#!/usr/bin/env python3

from pathlib import Path
from plumbum import local

# Runs clang-format on everything in the specified subdirs.
fmt = local["clang-format-12"]
subdirs = ["src", "test"]

for subdir in subdirs: 
    code_dir = Path.cwd() / subdir
    code_files = list(code_dir.glob("**/*.h")) + list(code_dir.glob("**/*.cpp"))

    for code_file in code_files: 
        print(f"Formatting file: {code_file}")
        fmt["-i", str(code_file)]()
