Mario Party 9
[![Build Status]][actions] [![Code Progress]][progress] [![Data Progress]][progress] [![Discord Badge]][discord]
=============

[Build Status]: https://github.com/ricky074game/marioparty9/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/ricky074game/marioparty9/actions/workflows/build.yml
[Code Progress]: https://decomp.dev/ricky074game/marioparty9.svg?mode=shield&measure=code&label=Code
[Data Progress]: https://decomp.dev/ricky074game/marioparty9.svg?mode=shield&measure=data&label=Data
[progress]: https://decomp.dev/ricky074game/marioparty9
[Discord Badge]: https://img.shields.io/discord/727908905392275526?color=%237289DA&logo=discord&logoColor=%23FFFFFF
[discord]: https://discord.gg/hKx3FJJgrV

A work-in-progress matching decompilation of *Mario Party 9* (Wii, NDcube, 2012).

The goal is C/C++ source that compiles with the original Metrowerks CodeWarrior
toolchain into a byte-identical copy of the game's code. Every linked file is
verified against the retail binary on each build, so the code stays
**shiftable**: once enough of it is decompiled, the game can be modified and
rebuilt freely.

This repository does **not** contain any game assets or assembly whatsoever. An
existing copy of the game is required.

Supported versions:

- `SSQP01`: Europe (En, Fr, De, Es, It)

Status
======

- `main.dol` is fully split and rebuilds byte-for-byte (`build/SSQP01/main.dol: OK`).
- The game's 115 REL modules (`files/modules/*.rel.lz`: boards, minigames,
  menus) are not tracked yet. They hold roughly 47 MB of code versus ~2.1 MB in
  `main.dol`, and will be added once the DOL is in good shape.

Progress
--------

Code matched and fully linked in `main.dol` (as of 2026-10-05; live numbers are on [decomp.dev][progress]):

| Area | Matched | Progress |
| --- | --- | --- |
| **Overall code** | 21,680 / 2,159,960 bytes | **1.00%** |
| **Overall data** | 2,224 / 1,130,636 bytes | **0.20%** |
| Game code | 0 / 1,495,896 bytes | 0.00% |
| SDK & middleware (RVL SDK, NW4R, HBM, ...) | 2,220 / 577,420 bytes | 0.38% |
| MSL_C (C standard library) | 16,844 / 54,708 bytes | 30.79% |
| Runtime / C++ support | 2,616 / 10,988 bytes | 23.81% |
| MetroTRK (debugger stub) | 0 / 20,644 bytes | 0.00% |

Dependencies
============

Windows
--------

On Windows, it's **highly recommended** to use native tooling. WSL or msys2 are **not** required.
When running under WSL, [objdiff](#diffing) is unable to get filesystem notifications for automatic rebuilds.

- Install [Python](https://www.python.org/downloads/) and add it to `%PATH%`.
  - Also available from the [Windows Store](https://apps.microsoft.com/store/detail/python-311/9NRWMJP3717K).
- Download [ninja](https://github.com/ninja-build/ninja/releases) and add it to `%PATH%`.
  - Quick install via pip: `pip install ninja`

macOS
------

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages):

  ```sh
  brew install ninja
  ```

[wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

Linux
------

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages).

[wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

Building
========

- Clone the repository:

  ```sh
  git clone https://github.com/ricky074game/marioparty9.git
  ```

- Copy your game's disc image to `orig/SSQP01`.
  - Supported formats: ISO (GCM), RVZ, WIA, WBFS, CISO, NFS, GCZ, TGC
  - After the initial build, the disc image can be deleted to save space.

- Configure:

  ```sh
  python configure.py
  ```

- Build:

  ```sh
  ninja
  ```

Diffing
=======

Once the initial build succeeds, an `objdiff.json` should exist in the project root.

Download the latest release from [encounter/objdiff](https://github.com/encounter/objdiff). Under project settings, set `Project directory`. The configuration should be loaded automatically.

Select an object from the left sidebar to begin diffing. Changes to the project will rebuild automatically: changes to source files, headers, `configure.py`, `splits.txt` or `symbols.txt`.

Ghidra
======

See [`tools/ghidra/README.md`](tools/ghidra/README.md) for the Ghidra setup and the script that imports `config/SSQP01/symbols.txt` into a Ghidra project.

Credits
=======

- Project skeleton and build system: [encounter/dtk-template](https://github.com/encounter/dtk-template), using [decomp-toolkit](https://github.com/encounter/decomp-toolkit) and [objdiff](https://github.com/encounter/objdiff).
- The MSL_C math sources are based on Sun Microsystems' freely distributable
  [fdlibm](http://www.netlib.org/fdlibm/) (license notices preserved in each file),
  adjusted to match Metrowerks' modified copy as shipped in the game.
