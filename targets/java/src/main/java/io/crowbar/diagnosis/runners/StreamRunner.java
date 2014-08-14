package io.crowbar.diagnosis.runners;


import io.crowbar.diagnosis.DiagnosticRequest;
import io.crowbar.diagnosis.DiagnosticSystem;
import io.crowbar.diagnosis.DiagnosticSystemRunner;

import flexjson.transformer.AbstractTransformer;
import flexjson.JSONSerializer;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;

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
}