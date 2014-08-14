package io.crowbar.diagnosis;

import io.crowbar.diagnosis.*;
import io.crowbar.diagnosis.runners.*;
import io.crowbar.diagnosis.runners.messages.*;
import io.crowbar.diagnosis.algorithms.*;
import io.crowbar.diagnosis.spectra.*;


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
            // StreamRunner sr = new StreamRunner(System.in,
            // System.out);

            // sr.run(j.create(),spectra);

            String original = Messages.serialize(DiagnosticMessages.issueRequest(j.create(), s));
            Request r = Messages.getRequestDeserializer().deserialize(original);
            String deserialized = Messages.serialize(r);

            System.out.println(original);
            System.out.println(deserialized);
        }
        catch (Throwable e) {e.printStackTrace();}
    }
}