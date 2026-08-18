// Ghidra headless script: export all decompiled functions to a single C file
// Usage: analyzeHeadless ... -postScript ExportDecompiled.java

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

import java.io.FileWriter;
import java.io.File;

public class ExportDecompiled extends GhidraScript {
    @Override
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        String outputPath = currentProgram.getExecutablePath();
        // Write to same directory as the binary
        File outFile = new File(getScriptArgs().length > 0
            ? getScriptArgs()[0]
            : "decompiled_output.c");

        FileWriter writer = new FileWriter(outFile);
        writer.write("/* Ghidra decompiled output -- " + currentProgram.getName() + " */\n\n");

        FunctionIterator funcs = currentProgram.getFunctionManager().getFunctions(true);
        int count = 0;
        while (funcs.hasNext()) {
            Function func = funcs.next();
            // Skip external/thunk functions
            if (func.isExternal() || func.isThunk()) continue;

            DecompileResults results = decomp.decompileFunction(func, 30, monitor);
            if (results.decompileCompleted()) {
                String code = results.getDecompiledFunction().getC();
                writer.write("/* Function: " + func.getName() + " @ " + func.getEntryPoint() + " */\n");
                writer.write(code);
                writer.write("\n\n");
                count++;
                println("Decompiled: " + func.getName());
            } else {
                writer.write("/* FAILED to decompile: " + func.getName() + " */\n\n");
            }
        }

        writer.close();
        decomp.dispose();
        println("Exported " + count + " functions to " + outFile.getAbsolutePath());
    }
}
