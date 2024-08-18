#!/usr/bin/env python

import subprocess
import argparse
from pathlib import Path
import shutil

CWD = Path(__file__).parent

def parse_args():
    parser = argparse.ArgumentParser("Project build helper")
    parser.add_argument("--build-dir", "-bd", type=Path, help="Build directory", default=CWD / "build")
    parser.add_argument("--build-type", "-bt", type=str, help="Build type", choices=["Debug", "Release"], default="Debug")
    return parser.parse_args()

def run_cmd(cmd: list) -> int:
    try:
        subprocess.check_output(cmd)
    except subprocess.CalledProcessError as exc:
        return exc.returncode
    return 0

def main():
    args = parse_args()
    bd = args.build_dir
    bt = args.build_type

    if bd.exists():
        shutil.rmtree(bd)
    
    ref = f"{str(CWD / '.')}"
    conan_install = ["conan", "install", ref, "-s", f"build_type={bt}", "--build=missing"]
    run_cmd(conan_install)
    conan_build = ["conan", "build", ref, "-s", f"build_type={bt}"]
    run_cmd(conan_build)

if __name__ == "__main__":
    main()
