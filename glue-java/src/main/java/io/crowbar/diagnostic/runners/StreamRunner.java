package io.crowbar.diagnostic.runners;


import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.runners.messages.DiagnosticMessages;
import io.crowbar.diagnostic.runners.messages.Messages;


import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;

public class StreamRunner implements Runner {
    private final InputStream in;
    private final PrintStream out;

    public StreamRunner (InputStream in,
                         OutputStream out) {
        this.in = in;
        this.out = new PrintStream(out, true);
    }

    @Override
    public final DiagnosticReport run (DiagnosticSystem system,
                                       Spectrum spectrum) throws ExecutionError {
        String jsonRequest = Messages.serialize(DiagnosticMessages.issueRequest(system, spectrum));


        out.println(jsonRequest);
        return null;
    }
}
