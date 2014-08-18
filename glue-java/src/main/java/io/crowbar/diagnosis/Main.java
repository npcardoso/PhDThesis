package io.crowbar.diagnosis;

import io.crowbar.diagnosis.*;
import io.crowbar.diagnosis.runners.*;
import io.crowbar.diagnosis.runners.messages.*;
import io.crowbar.diagnosis.algorithms.*;
import io.crowbar.diagnosis.spectra.*;
import com.sun.jna.ptr.PointerByReference;
import com.sun.jna.Library;
import com.sun.jna.Pointer;
import com.sun.jna.Native;

import flexjson.JSONSerializer;
import flexjson.transformer.AbstractTransformer;


public class Main {
    public static void main (String args[]) {
        Spectra s = new EditableSpectra();

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();


        j.addGenerator(new SingleFaultGenerator());
        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        j.addConnection(new Connection(0, 0));

        j.addGenerator(new MHSGenerator());
        j.addRanker(new FuzzinelRanker());
        j.addConnection(new Connection(0, 1));
        j.addConnection(new Connection(1, 1));

        try {
            JNARunner runner = new JNARunner();

            runner.run(j.create(), s);
        }
        catch (Throwable e) {e.printStackTrace();}
    }
}
