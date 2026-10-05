// Runs auto-analysis only over the REL module's memory blocks (everything that is not
// the DOL "MAIN_*" blocks or hardware-register blocks). Intended for programs imported
// with -noanalysis through the GameCube loader (DOL + one REL in a single program).
//@category MarioParty9
import ghidra.app.script.GhidraScript;
import ghidra.app.plugin.core.analysis.AutoAnalysisManager;
import ghidra.program.model.address.*;
import ghidra.program.model.mem.*;

public class AnalyzeRelBlocks extends GhidraScript {
    @Override
    protected void run() throws Exception {
        AddressSet set = new AddressSet();
        for (MemoryBlock m : currentProgram.getMemory().getBlocks()) {
            long a = m.getStart().getOffset();
            if (m.getName().startsWith("MAIN_") || a < 0x80000000L || a >= 0xCC000000L) continue;
            set.add(m.getStart(), m.getEnd());
            if (m.isExecute()) disassemble(m.getStart());
        }
        println("AnalyzeRelBlocks: analyzing " + set.getNumAddresses() + " bytes");
        AutoAnalysisManager mgr = AutoAnalysisManager.getAnalysisManager(currentProgram);
        mgr.reAnalyzeAll(set);
        mgr.startAnalysis(monitor);
        println("AnalyzeRelBlocks: functions now = " + currentProgram.getFunctionManager().getFunctionCount());
    }
}
