# Imports + analyzes REL modules into the MarioParty9 Ghidra project under /modules.
# Each REL is staged next to main.dol so the GameCube loader relocates it against the DOL
# (the loader merges every .dol/.rel found in the import directory, hence one stage dir per REL).
# Usage: .\ImportRels.ps1 [-Names mg9101,menu] [-Force]     (default: all modules not yet imported)
param([string[]]$Names, [switch]$Force)
$ErrorActionPreference = 'Continue'
$base   = 'C:\Users\Ricky\Desktop\Codes\Cool Coding'
$repo   = "$base\Marioparty9decomp"
$ghidra = "$base\tools\ghidra\ghidra_12.1.4_PUBLIC"
$proj   = "$base\mp9_ghidra"
$mods   = "$base\mp9_files\modules"
$stage  = "$base\mp9_files\_stage"
$state  = "$base\mp9_ghidra\imported_modules.txt"
$env:JAVA_HOME = 'C:\Program Files\Eclipse Adoptium\jdk-21.0.9.10-hotspot'
if ($Names) { $Names = $Names -split "," }
$done = @(); if ((Test-Path $state) -and -not $Force) { $done = Get-Content $state }
$list = if ($Names) { $Names | % { Get-Item "$mods\$_.rel" } } else { Get-ChildItem "$mods\*.rel" }
foreach ($rel in $list) {
  $n = $rel.BaseName
  if ($done -contains $n) { continue }
  Remove-Item $stage -Recurse -Force -ErrorAction SilentlyContinue
  New-Item -ItemType Directory -Force $stage | Out-Null
  Copy-Item "$base\mp9_files\main.dol" $stage; Copy-Item $rel.FullName $stage
  $t = Get-Date
  & "$ghidra\support\analyzeHeadless.bat" $proj MarioParty9/modules -import "$stage\$($rel.Name)" -overwrite -noanalysis `
     -loader-autoloadMaps false -scriptPath "$repo\tools\ghidra" `
     -postScript ImportDtkSymbols.java "$repo\config\SSQP01\symbols.txt" labels `
     -postScript AnalyzeRelBlocks.java -log "$base\mp9_ghidra\import_$n.log" *> "$base\mp9_ghidra\import_$n.out"
  $ok = Select-String -Path "$base\mp9_ghidra\import_$n.out" -Pattern 'REPORT: Import succeeded' -Quiet
  if ($ok) { Add-Content $state $n }
  "{0} {1} ok={2} {3:n0}s" -f (Get-Date -Format s), $n, $ok, ((Get-Date)-$t).TotalSeconds | Add-Content "$base\mp9_ghidra\import_progress.log"
}
Remove-Item $stage -Recurse -Force -ErrorAction SilentlyContinue
"ALL DONE $(Get-Date -Format s)" | Add-Content "$base\mp9_ghidra\import_progress.log"
