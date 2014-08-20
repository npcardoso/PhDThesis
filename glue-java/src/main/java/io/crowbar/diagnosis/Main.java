package io.crowbar.diagnosis;

import io.crowbar.diagnosis.*;
import io.crowbar.diagnosis.runners.*;
import io.crowbar.diagnosis.runners.messages.*;
import io.crowbar.diagnosis.algorithms.*;
import io.crowbar.diagnosis.spectra.*;
import io.crowbar.diagnosis.spectra.unserializers.*;

import com.sun.jna.ptr.PointerByReference;
import com.sun.jna.Library;
import com.sun.jna.Pointer;
import com.sun.jna.Native;

import flexjson.JSONSerializer;
import flexjson.JSONDeserializer;
import flexjson.transformer.AbstractTransformer;
import java.util.Collection;
import java.util.ArrayList;
import java.util.Scanner;


public class Main {
    public static void main (String args[]) {
        Spectra s = HitSpectraUnserializer.unserialize(new Scanner("3 2 1 1 0 1 1 0 1 -"));
        TransactionFactory tf = new TransactionFactory();

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();


        j.addGenerator(new SingleFaultGenerator());
        j.addGenerator(new MHSGenerator());


        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        j.addConnection(new Connection(0, 0));
        j.addConnection(new Connection(1, 0));

        j.addRanker(new FuzzinelRanker());
        j.addConnection(new Connection(1, 1));


        try {
            JNARunner runner = new JNARunner();

            System.out.println("------ Inside JNA --------");
            DiagnosticReport dr = runner.run(j.create(), s);
            System.out.println("------ Inside JNA --------");

            System.out.println("DiagnosticReport After deserialization: ");
            System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));
        }
        catch (Throwable e) {e.printStackTrace();}
    }
}
