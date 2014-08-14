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
    public interface LibDiag extends Library {
        String run_diagnostic (String request, PointerByReference response);
        void cleanup_diagnostic (Pointer response);
    }

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

            String original = new JSONSerializer().deepSerialize(j.create()); // Messages.serialize(DiagnosticMessages.issueRequest(j.create(), s));
            LibDiag libdiag = (LibDiag) Native.loadLibrary("diag", LibDiag.class);

            PointerByReference ptrRef = new PointerByReference();
            libdiag.run_diagnostic(original, ptrRef);
            Pointer p = ptrRef.getValue();
            String diagnostic = p.getString(0);
            System.out.println("diag: " + diagnostic);
            libdiag.cleanup_diagnostic(p);

            // Request r = Messages.getRequestDeserializer().deserialize(original);
            // String deserialized = Messages.serialize(r);

            // String withError = "{\"spectra\":\"0 0\",\"system\":{\"connections\":[{\"from\":0,\"to\":0},{\"from\":0},{\"from\":1,\"to\":1}],\"generators\":[{\"name\":\"single_fault\"},{\"name\":\"mhs2\"}],\"rankers\":[{\"configs\":{\"type\":\"ochiai\"},\"name\":\"similarity\"},{\"name\":\"fuzzinel\"}]},\"type\":\"diagnostic\"}";

            // Request r2 = Messages.getRequestDeserializer().deserialize(withError);
            // String deserialized2 = Messages.serialize(r2);

            // System.out.println(original);
            // System.out.println(deserialized);
            // System.out.println(deserialized2);
        }
        catch (Throwable e) {e.printStackTrace();}
    }
}