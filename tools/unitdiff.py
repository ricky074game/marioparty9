#!/usr/bin/env python3

###
# Compiles one source file and diffs its functions against the original
# (target) object of a split, without going through ninja. Safe to run several
# instances in parallel.
#
# Usage:
#   python tools/unitdiff.py <unit> [--module mg9101] [--mw Wii/1.3] [--cflags "..."]
#       [--all] [--context N]
#
# Example:
#   python tools/unitdiff.py game/common/unk_170B0.cpp
#
# Functions are paired by order: the Nth function in the source object is
# compared with the Nth function of the target object. Relocation targets are
# shown but not compared by name, only by kind.
#
# The compiler flags default to the ones configure.py wrote to build.ninja for
# this source file; --mw / --cflags override them for experiments.
###

import argparse
import difflib
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).resolve().parent.parent
OBJDUMP = ROOT / "build" / "binutils" / "powerpc-eabi-objdump.exe"
if not OBJDUMP.exists():
    OBJDUMP = ROOT / "build" / "binutils" / "powerpc-eabi-objdump"


def ninja_flags(src: str) -> Tuple[str, str]:
    text = (ROOT / "build.ninja").read_text().replace("$\n", "")
    want = src.replace("/", "\\")
    pattern = re.compile(
        r"^build [^\n]*: mwcc\w* *\n?\s*" + re.escape(want) + r"[^\n]*\n((?:  [^\n]*\n)+)",
        re.M,
    )
    m = pattern.search(text)
    if not m:
        sys.exit(f"No build rule for {src} in build.ninja (run configure.py)")
    block = m[1]
    mw = re.search(r"mw_version = (\S+)", block)[1].replace("\\", "/")
    cflags = re.search(r"cflags = (.*)", block)[1]
    cflags = re.sub(r"\s+", " ", cflags)
    return mw, cflags


def compile_source(src: Path, mw: str, cflags: str, out: Path) -> None:
    cmd = (
        f'"{ROOT / "build" / "tools" / "sjiswrap.exe"}" '
        f'"{ROOT / "build" / "compilers" / mw / "mwcceppc.exe"}" '
        f'{cflags} -c "{src}" -o "{out}"'
    )
    if os.name != "nt":
        wibo = ROOT / "build" / "tools" / "wibo"
        cmd = f'"{wibo}" ' + cmd
    r = subprocess.run(cmd, shell=True, capture_output=True, text=True, cwd=ROOT)
    if r.returncode != 0 or not out.exists():
        print(r.stdout + r.stderr)
        sys.exit("Compile failed")


def disassemble(obj: Path) -> Dict[str, List[str]]:
    out = subprocess.run(
        [str(OBJDUMP), "-dr", "--no-show-raw-insn", "-M", "gekko", str(obj)],
        capture_output=True,
        text=True,
    ).stdout
    funcs: Dict[str, List[str]] = {}
    order: List[str] = []
    cur: Optional[List[str]] = None
    base = 0
    for line in out.splitlines():
        m = re.match(r"^([0-9a-f]+) <(.*)>:", line)
        if m:
            cur = []
            base = int(m[1], 16)
            funcs[m[2]] = cur
            order.append(m[2])
            continue
        if cur is None:
            continue
        m = re.match(r"\s+[0-9a-f]+:\s+(.*)", line)
        if not m:
            continue
        ins = re.sub(r"\s+", " ", m[1]).strip()
        if ins.startswith("R_PPC"):
            kind = ins.split(" ")[0][7:]
            target = ins.split(" ")[1] if " " in ins else ""
            if cur:
                cur[-1] = cur[-1] + f"  [{kind}]"
                cur[-1] = cur[-1] + f"  ; {target}"
            continue
        ins = re.sub(r"<[^>]*>", "", ins).strip()
        # Make local branch targets relative to the function start
        b = re.match(r"^(b\w*[+-]?)\s+((?:cr\d,)?)([0-9a-f]+)$", ins)
        if b:
            ins = f"{b[1]} {b[2]}.+0x{int(b[3], 16) - base:x}"
        cur.append(ins)
    return {name: funcs[name] for name in order}


def weak_symbols(obj: Path) -> set:
    nm = OBJDUMP.with_name(OBJDUMP.name.replace("objdump", "nm"))
    out = subprocess.run([str(nm), str(obj)], capture_output=True, text=True).stdout
    return {l.split()[-1] for l in out.splitlines() if len(l.split()) >= 3 and l.split()[-2] in ("W", "V")}


def strip_comment(line: str) -> str:
    return line.split("  ; ")[0]


def drop_padding(lines: List[str]) -> List[str]:
    # Target objects include alignment padding as zero words ("invalid"/".long 0")
    while lines and (lines[-1].startswith(".long 0x0") or lines[-1] in ("nop", ".long 0x00000000")):
        lines = lines[:-1]
    return lines


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("unit", help="source path relative to src/, e.g. game/common/unk_170B0.cpp")
    parser.add_argument("--module", default="mg9101")
    parser.add_argument("--mw", help="compiler version override, e.g. Wii/1.7")
    parser.add_argument("--cflags", help="full cflags override")
    parser.add_argument("--all", action="store_true", help="print full disassembly of mismatching functions")
    parser.add_argument("--context", type=int, default=3)
    args = parser.parse_args()

    src = Path("src") / args.unit
    mw, cflags = ninja_flags(str(src))
    if args.mw:
        mw = args.mw
    if args.cflags:
        cflags = args.cflags
    stem = Path(args.unit).with_suffix(".o")
    target = ROOT / "build" / "SSQP01" / (args.module if args.module != "main" else "") / "obj" / stem
    if args.module == "main":
        target = ROOT / "build" / "SSQP01" / "obj" / stem
    if not target.exists():
        sys.exit(f"No target object {target} (run ninja once)")

    with tempfile.TemporaryDirectory() as tmp:
        ours_obj = Path(tmp) / "ours.o"
        compile_source(ROOT / src, mw, cflags, ours_obj)
        ours = disassemble(ours_obj)
        # Weak functions (out-of-line copies of inline functions) are dropped by
        # the linker in the original build, so don't pair them by position.
        weak = weak_symbols(ours_obj)
        ours = {k: v for k, v in ours.items() if k not in weak}
    theirs = disassemble(target)

    t_names = [n for n in theirs if not n.startswith("gap_")]
    o_names = list(ours)
    total = matched = 0
    print(f"{args.unit} vs {args.module} ({mw})")
    for i, tn in enumerate(t_names):
        tl = drop_padding(theirs[tn])
        size = len(tl) * 4
        total += size
        if i >= len(o_names):
            print(f"  MISSING  {tn} ({size} bytes)")
            continue
        on = o_names[i]
        ol = drop_padding(ours[on])
        ta = [strip_comment(x) for x in tl]
        oa = [strip_comment(x) for x in ol]
        if ta == oa:
            matched += size
            print(f"  OK       {tn} <- {on} ({size} bytes)")
            continue
        sm = difflib.SequenceMatcher(None, ta, oa)
        print(f"  DIFF {sm.ratio()*100:5.1f}% {tn} <- {on} ({size} bytes, ours {len(ol)*4})")
        if args.all:
            for line in difflib.unified_diff(tl, ol, "target", "ours", n=args.context, lineterm=""):
                print("      " + line)
    for on in o_names[len(t_names):]:
        print(f"  EXTRA    {on}")
    print(f"matched {matched}/{total} bytes")


if __name__ == "__main__":
    main()
