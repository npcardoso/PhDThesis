package io.crowbar.diagnostic.runners;


import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.messages.DiagnosticMessages;
import io.crowbar.messages.Messages;


import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;

public class StreamRunner implements Runner {
    private final InputStreamReader in;
    private final PrintStream out;

    public StreamRunner (InputStream in,
                         OutputStream out) {
        this.in = new InputStreamReader(in);
        this.out = new PrintStream(out, true);
    }

    @Override
    public final DiagnosticReport run (DiagnosticSystem system,
                                       Spectrum spectrum) throws ExecutionError {
        String jsonRequest = Messages.serialize(DiagnosticMessages.issueRequest(system, spectrum));
        out.println(jsonRequest);
        DiagnosticMessages.Response response = (DiagnosticMessages.Response)
            Messages.getResponseDeserializer().deserialize(in);

        return response.getReport();

    }
}
