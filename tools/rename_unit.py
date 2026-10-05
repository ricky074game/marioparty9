#!/usr/bin/env python3

###
# Renames the functions of a split unit in every module that contains it.
#
# Usage:
#   python tools/rename_unit.py <unit> <names file>
#
# The names file has one line per function of the unit, in address order:
#   <reference name> <new name>
# Only the second column is used; the Nth function (by address) inside the
# unit's .text split is renamed to the Nth name, in each module.
###

import re
import sys
from pathlib import Path
from typing import List, Tuple

CONFIG = Path("config") / "SSQP01"
SYM_RE = re.compile(r"^(\S+) = \.text:0x([0-9A-F]+); // type:function")


def unit_ranges(splits: str, unit: str) -> List[Tuple[int, int]]:
    m = re.search(rf"^{re.escape(unit)}:[^\n]*\n((?:\t[^\n]*\n?)+)", splits, re.M)
    if not m:
        return []
    ranges = []
    for line in m[1].splitlines():
        s = re.match(r"\t\.text\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)", line)
        if s:
            ranges.append((int(s[1], 16), int(s[2], 16)))
    return ranges


def main() -> None:
    unit, names_path = sys.argv[1], Path(sys.argv[2])
    names = [l.split()[-1] for l in names_path.read_text().splitlines() if l.strip() and not l.startswith("#")]
    count = 0
    for splits_path in [CONFIG / "splits.txt", *sorted(CONFIG.glob("*/splits.txt"))]:
        ranges = unit_ranges(splits_path.read_text(), unit)
        if not ranges:
            continue
        sym_path = splits_path.parent / "symbols.txt"
        lines = sym_path.read_text().splitlines()
        funcs = []
        for i, line in enumerate(lines):
            m = SYM_RE.match(line)
            if m:
                addr = int(m[2], 16)
                if any(a <= addr < b for a, b in ranges):
                    funcs.append((addr, i, m[1]))
        funcs.sort()
        if len(funcs) != len(names):
            print(f"{splits_path.parent.name}: {len(funcs)} functions, expected {len(names)}; skipped")
            continue
        for (addr, i, old), new in zip(funcs, names):
            lines[i] = new + lines[i][len(old):]
        sym_path.write_text("\n".join(lines) + "\n")
        count += 1
    print(f"{unit}: renamed in {count} modules")


if __name__ == "__main__":
    main()
