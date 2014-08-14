package io.crowbar.diagnosis.runners;

import io.crowbar.diagnosis.algorithms.*;
import io.crowbar.diagnosis.spectra.*;
import io.crowbar.diagnosis.Connection;

import io.crowbar.diagnosis.DiagnosticRequest;
import io.crowbar.diagnosis.DiagnosticSystem;
import io.crowbar.diagnosis.DiagnosticSystemRunner;
import io.crowbar.diagnosis.DiagnosticSystemFactory;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import flexjson.transformer.AbstractTransformer;
import flexjson.JSONSerializer;

public class StreamRunner implements DiagnosticSystemRunner {
    private static class ExcludeTransformer extends AbstractTransformer {
        @Override
        public Boolean isInline () {return true;}

        @Override
        public void transform (Object object) {}
    }

    private final InputStream in;
    private final PrintStream out;

    public StreamRunner (InputStream in,
                         OutputStream out) {
        this.in = in;
        this.out = new PrintStream(out, true);
    }

    @Override
    public DiagnosticSystem getServerInterpretation (DiagnosticSystem system) throws ExecutionError {
        return system;
    }

    @Override
    public final void run (DiagnosticRequest request) throws ExecutionError {
        String jsonRequest = new JSONSerializer()
                             .transform(new ExcludeTransformer(), void.class) // Remove null(optional) stuff
                             .exclude("*.class")
                             .deepSerialize(request);


        out.println(jsonRequest);
    }

    public static void main (String[] args) {
        try {
            DiagnosticSystemFactory j = new DiagnosticSystemFactory();

            j.addGenerator(new SingleFaultGenerator());
            j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
            j.addConnection(new Connection(0, 0));

            j.addGenerator(new MHSGenerator());
            j.addRanker(new FuzzinelRanker());
            j.addConnection(new Connection(0, 1));
            j.addConnection(new Connection(1, 1));

            DiagnosticRequest dr = new DiagnosticRequest(j.create(),
                                                         new EditableSpectra());

            StreamRunner sr = new StreamRunner(System.in,
                                               System.out);

            sr.run(dr);
        }
        catch (Throwable e) {e.printStackTrace();}
    }
}