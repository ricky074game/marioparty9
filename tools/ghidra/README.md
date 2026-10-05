# Ghidra setup for Mario Party 9 (SSQP01)

## Locations
- Ghidra 12.1.4 (needs JDK 21): `C:\Users\Ricky\Desktop\Codes\Cool Coding\tools\ghidra\ghidra_12.1.4_PUBLIC`
  - GUI: `ghidraRun.bat` in that folder.
- GameCube/Wii loader (Cuyler36/Ghidra-GameCube-Loader 1.3.1, built for Ghidra 12.1, works on 12.1.4):
  `...\ghidra_12.1.4_PUBLIC\Ghidra\Extensions\GameCubeLoader` (active for GUI and headless).
- Binaries: `C:\Users\Ricky\Desktop\Codes\Cool Coding\mp9_files\main.dol` and `...\mp9_files\modules\*.rel` (115 decompressed RELs).
- Project: `C:\Users\Ricky\Desktop\Codes\Cool Coding\mp9_ghidra\MarioParty9.gpr`
  - `/main.dol`: DOL, fully analyzed, with names from `config/SSQP01/symbols.txt`.
  - `/modules/<name>.rel`: each program holds main.dol + that REL (relocated against the DOL).

Close the GUI before running headless commands (the project is locked while open).

## Re-apply symbols after symbols.txt changes
```
set JAVA_HOME=C:\Program Files\Eclipse Adoptium\jdk-21.0.9.10-hotspot
"C:\Users\Ricky\Desktop\Codes\Cool Coding\tools\ghidra\ghidra_12.1.4_PUBLIC\support\analyzeHeadless.bat" ^
  "C:\Users\Ricky\Desktop\Codes\Cool Coding\mp9_ghidra" MarioParty9 -process main.dol ^
  -scriptPath "C:\Users\Ricky\Desktop\Codes\Cool Coding\Marioparty9decomp\tools\ghidra" ^
  -preScript ImportDtkSymbols.java "C:\Users\Ricky\Desktop\Codes\Cool Coding\Marioparty9decomp\config\SSQP01\symbols.txt"
```
`-preScript` means analysis re-runs afterwards. The script creates functions (with sizes) for `type:function`
and labels for everything else; it is idempotent. Add a 2nd script arg `labels` to only create labels.
In the GUI: Script Manager, add the `tools/ghidra` dir, run `ImportDtkSymbols.java`.

## Importing more modules
The loader merges every .dol/.rel in the import directory into one program, so `ImportRels.ps1` stages
main.dol + one REL per import, applies DOL symbols as labels, and analyzes only the REL blocks
(about 1-2 min each):
```
powershell -ExecutionPolicy Bypass -File tools\ghidra\ImportRels.ps1                 # all not yet imported
powershell -ExecutionPolicy Bypass -File tools\ghidra\ImportRels.ps1 -Names mg9101,menu -Force
```
Progress: `mp9_ghidra\import_progress.log`; imported names: `mp9_ghidra\imported_modules.txt`.
Notes: REL-to-REL relocations are not resolved (only DOL imports are); REL programs are laid out right after the DOL (first REL block at ~0x803344A0).
