package io.crowbar.diagnostic;

import io.crowbar.diagnostic.runners.*;
import io.crowbar.diagnostic.runners.messages.*;
import io.crowbar.diagnostic.algorithms.*;
import io.crowbar.diagnostic.spectrum.*;
import io.crowbar.diagnostic.spectrum.unserializers.*;

import flexjson.JSONSerializer;
import flexjson.JSONDeserializer;
import java.util.Scanner;


public class Main {
    public static void main (String[] args) {
        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner("3 2 1 1 0 1 1 0 1 -"));
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
