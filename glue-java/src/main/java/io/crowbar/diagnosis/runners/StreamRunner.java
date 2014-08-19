package io.crowbar.diagnosis.runners;


import io.crowbar.diagnosis.DiagnosticSystem;
import io.crowbar.diagnosis.DiagnosticReport;
import io.crowbar.diagnosis.Runner;
import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.runners.messages.DiagnosticMessages;
import io.crowbar.diagnosis.runners.messages.Messages;


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
    public DiagnosticSystem getServerInterpretation (DiagnosticSystem system) throws ExecutionError {
        return system;
    }

    @Override
    public final DiagnosticReport run (DiagnosticSystem system,
                                       Spectra spectra) throws ExecutionError {
        String jsonRequest = Messages.serialize(DiagnosticMessages.issueRequest(system, spectra));


        out.println(jsonRequest);
        return null;
    }
}
