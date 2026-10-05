// Applies a decomp-toolkit symbols.txt to the current program (main.dol).
// Usage (headless): -postScript ImportDtkSymbols.java "<path to symbols.txt>" [labels]
// Optional 2nd arg "labels": do not create functions, label everything (much faster; used for REL programs).
// Lines: name = .section:0xADDR; // type:function size:0x.. scope:global
//@category MarioParty9
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

public class ImportDtkSymbols extends GhidraScript {
    private static final Pattern LINE = Pattern.compile(
        "^\\s*(\\S+)\\s*=\\s*([.\\w]+):0x([0-9A-Fa-f]+);\\s*(?://\\s*(.*))?$");

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File f = (args.length > 0) ? new File(args[0]) : askFile("dtk symbols.txt", "Open");
        List<String> lines = Files.readAllLines(f.toPath());
        boolean labelsOnly = args.length > 1 && args[1].equals("labels");
        int funcs = 0, labels = 0, renamed = 0, failed = 0, skipped = 0;
        FunctionManager fm = currentProgram.getFunctionManager();
        SymbolTable st = currentProgram.getSymbolTable();
        for (String line : lines) {
            if (monitor.isCancelled()) break;
            Matcher m = LINE.matcher(line);
            if (!m.matches()) { continue; }
            String name = m.group(1);
            long addrVal = Long.parseUnsignedLong(m.group(3), 16);
            String attrs = m.group(4) == null ? "" : m.group(4);
            Address addr = toAddr(addrVal);
            if (!currentProgram.getMemory().contains(addr)) { skipped++; continue; }
            boolean isFunc = attrs.contains("type:function") && !labelsOnly;
            long size = 0;
            Matcher sm = Pattern.compile("size:0x([0-9A-Fa-f]+)").matcher(attrs);
            if (sm.find()) size = Long.parseLong(sm.group(1), 16);
            try {
                if (isFunc) {
                    Function fn = fm.getFunctionAt(addr);
                    if (fn == null) {
                        // remove default-named function overlapping this range start, then (re)create
                        Function cont = fm.getFunctionContaining(addr);
                        if (cont != null && !cont.getEntryPoint().equals(addr)) {
                            // keep going: createFunction below will fail if overlap; fallback to label
                        }
                        disassemble(addr);
                        if (size > 0) {
                            try {
                                AddressSet body = new AddressSet(addr, addr.add(size - 1));
                                fn = fm.createFunction(name, addr, body, SourceType.USER_DEFINED);
                            } catch (Exception e) { fn = null; }
                        }
                        if (fn == null) fn = createFunction(addr, name);
                        if (fn != null) { funcs++; continue; }
                        // fallback: label only
                        st.createLabel(addr, name, SourceType.USER_DEFINED);
                        labels++;
                    } else {
                        if (!fn.getName().equals(name)) {
                            fn.setName(name, SourceType.USER_DEFINED);
                            renamed++;
                        }
                        funcs++;
                    }
                } else {
                    Symbol prim = st.getPrimarySymbol(addr);
                    if (prim != null && prim.getName().equals(name)) { labels++; continue; }
                    Symbol s = st.createLabel(addr, name, SourceType.USER_DEFINED);
                    s.setPrimary();
                    labels++;
                }
            } catch (Exception e) {
                failed++;
                println("WARN: " + name + " @ " + addr + ": " + e.getMessage());
            }
        }
        println("ImportDtkSymbols: functions=" + funcs + " (renamed existing=" + renamed + ") labels=" + labels
            + " skipped(not in memory)=" + skipped + " failed=" + failed);
    }
}
