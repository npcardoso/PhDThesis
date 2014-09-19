package io.crowbar.diagnostic;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.messages.Messages;
import io.crowbar.messages.VisualizationMessages;

import com.sun.net.httpserver.HttpExchange;
import java.io.OutputStream;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

class DiagnosticReportHandler extends AbstractHttpHandler {
    private static class Entry {
        private final DiagnosticSystem ds;
        private final Spectrum spectrum;
        private final DiagnosticReport dr;

        public DiagnosticSystem getDiagnosticSystem () {
            return ds;
        }

        public Spectrum getSpectrum () {
            return spectrum;
        }

        public DiagnosticReport getDiagnosticReport () {
            return dr;
        }

        public Entry (DiagnosticSystem ds,
                      Spectrum spectrum,
                      DiagnosticReport dr) {
            this.ds = ds;
            this.spectrum = spectrum;
            this.dr = dr;
        }
    }

    @Override
    public void handle (HttpExchange t,
                        String relativePath)
    throws Exception {
        String response = "";


        if (relativePath.equals("/")) {
            for (String id : entries.keySet())
                response += t.getHttpContext().getPath() + "/" + id + "\n";
        } else {
            Pattern p = Pattern.compile("/([^/]*)/([0-9]*)");
            Matcher m = p.matcher(relativePath);
            m.matches();
            String id = m.group(1);
            int connection = Integer.parseInt(m.group(2));


            Entry e = entries.get(id);
            DiagnosticSystem diagSystem = e.getDiagnosticSystem();
            Spectrum spectrum = e.getSpectrum();
            DiagnosticReport dr = e.getDiagnosticReport();
            Connection c = diagSystem.getConnections().get(connection);
            Diagnostic diag = dr.getDiagnostic(c);

            List<Double> scores = spectrum.getScorePerNode(diag, Spectrum.SUM);
            Tree tr = spectrum.getTree();

            response = io.crowbar.messages.Messages.serialize(
                VisualizationMessages.issueRequest(tr, scores));
        }

        System.out.println("Response:" + response);

        t.sendResponseHeaders(200, response.length());
        OutputStream os = t.getResponseBody();
        os.write(response.getBytes());
        os.close();
    }

    private final Map<String, Entry> entries = new HashMap<String, Entry> ();

    public Entry handle (String id,
                         DiagnosticSystem ds,
                         Spectrum spectrum,
                         DiagnosticReport dr) {
        Entry e = new Entry(ds, spectrum, dr);


        entries.put(id, e);
        return e;
    }
}