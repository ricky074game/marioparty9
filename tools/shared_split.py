#!/usr/bin/env python3

###
# Applies a split to every module that contains an identical copy of a block
# of functions.
#
# Many source files are statically linked into most of the game's REL modules,
# so the same code appears (at different addresses) in up to ~115 modules.
# This finds the block given by a reference module and function range in every
# other module, by comparing normalized disassembly, and adds a split for it.
#
# Usage:
#   python tools/shared_split.py <unit path> <ref module> <first fn> <last fn>
#       [--rename names.txt] [--dry-run]
#
# Example:
#   python tools/shared_split.py game/Foo.cpp mg9101 fn_18_170B0 fn_18_17200
#
# --rename takes a file of "<ref symbol> <new name>" lines. The functions of
# the block are renamed to those names in every module, so objdiff can pair
# them with the decompiled functions.
#
# Requires an up-to-date `ninja` (for build/SSQP01/*/asm).
###

import argparse
import hashlib
import re
from pathlib import Path
from typing import Dict, List, Optional, Tuple

VERSION = "SSQP01"
CONFIG = Path("config") / VERSION
BUILD = Path("build") / VERSION

FN_RE = re.compile(r"^\.fn (\S+), (\w+)")
INS_RE = re.compile(r"^/\* ([0-9A-F]+) [0-9A-F]+  [0-9A-F ]+ \*/\t(.*)")
NORMALIZE = [
    (re.compile(r"\bfn_\d+_[0-9A-F]+|\bfn_[0-9A-F]{8}"), "FN"),
    (re.compile(r"\blbl_\d+_\w+|\blbl_[0-9A-F]{8}"), "LBL"),
    (re.compile(r"\.L_\w+"), "L"),
    (re.compile(r'"?@\d+(_[0-9A-F]+)?"?'), "ANON"),
]
SYM_RE = re.compile(r"^(\S+) = \.text:0x([0-9A-F]+); // type:function size:0x([0-9A-F]+)")

Func = Tuple[str, int, int, str]  # name, address, size, hash


def module_dir(module: str) -> Path:
    return BUILD / "asm" if module == "main" else BUILD / module / "asm"


def symbols_path(module: str) -> Path:
    return CONFIG / "symbols.txt" if module == "main" else CONFIG / module / "symbols.txt"


def splits_path(module: str) -> Path:
    return CONFIG / "splits.txt" if module == "main" else CONFIG / module / "splits.txt"


def load_functions(module: str) -> List[Func]:
    sizes: Dict[str, Tuple[int, int]] = {}
    for line in symbols_path(module).read_text().splitlines():
        m = SYM_RE.match(line)
        if m:
            sizes[m[1]] = (int(m[2], 16), int(m[3], 16))

    funcs: List[Func] = []
    for path in module_dir(module).glob("*.s"):
        cur: Optional[str] = None
        body: List[str] = []
        for line in path.read_text(errors="replace").splitlines():
            m = FN_RE.match(line)
            if m:
                cur, body = m[1], []
                continue
            if cur is None:
                continue
            if line.startswith(".endfn"):
                if cur in sizes and body:
                    h = hashlib.sha1("\n".join(body).encode()).hexdigest()
                    funcs.append((cur, sizes[cur][0], sizes[cur][1], h))
                cur = None
                continue
            mi = INS_RE.match(line)
            if mi:
                text = mi[2]
                for regex, repl in NORMALIZE:
                    text = regex.sub(repl, text)
                body.append(text)
    funcs.sort(key=lambda f: f[1])
    return funcs


def find_block(funcs: List[Func], hashes: List[str]) -> Optional[List[Func]]:
    n = len(hashes)
    for i in range(len(funcs) - n + 1):
        if all(funcs[i + j][3] == hashes[j] for j in range(n)):
            return funcs[i : i + n]
    return None


def all_modules() -> List[str]:
    mods = [p.name for p in CONFIG.iterdir() if (p / "splits.txt").exists()]
    return ["main"] + sorted(mods)


def add_split(module: str, unit: str, start: int, end: int) -> bool:
    path = splits_path(module)
    text = path.read_text()
    if re.search(rf"^{re.escape(unit)}:", text, re.M):
        return False
    fmt = "0x{:08X}" if module == "main" else "0x{:08X}"
    block = f"\n{unit}:\n\t.text       start:{fmt.format(start)} end:{fmt.format(end)}\n"
    path.write_text(text.rstrip("\n") + "\n" + block)
    return True


def rename(module: str, mapping: Dict[str, str]) -> None:
    path = symbols_path(module)
    lines = path.read_text().splitlines()
    out = []
    for line in lines:
        name = line.split(" ", 1)[0]
        if name in mapping:
            line = mapping[name] + line[len(name):]
        out.append(line)
    path.write_text("\n".join(out) + "\n")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("unit")
    parser.add_argument("ref_module")
    parser.add_argument("first")
    parser.add_argument("last")
    parser.add_argument("--rename", type=Path)
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    ref = load_functions(args.ref_module)
    names = [f[0] for f in ref]
    i, j = names.index(args.first), names.index(args.last)
    block = ref[i : j + 1]
    hashes = [f[3] for f in block]

    new_names: List[Optional[str]] = [None] * len(block)
    if args.rename:
        by_ref = {}
        for line in args.rename.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                old, new = line.split()
                by_ref[old] = new
        new_names = [by_ref.get(f[0]) for f in block]

    found = 0
    for module in all_modules():
        funcs = ref if module == args.ref_module else load_functions(module)
        match = find_block(funcs, hashes)
        if match is None:
            continue
        found += 1
        start = match[0][1]
        end = match[-1][1] + match[-1][2]
        if args.dry_run:
            print(f"{module}: 0x{start:X}-0x{end:X}")
            continue
        add_split(module, args.unit, start, end)
        mapping = {f[0]: n for f, n in zip(match, new_names) if n is not None}
        if mapping:
            rename(module, mapping)
    print(f"{args.unit}: {len(block)} functions, {sum(f[2] for f in block)} bytes, found in {found} modules")


if __name__ == "__main__":
    main()
