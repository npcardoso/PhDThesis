package io.crowbar.diagnostic;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.messages.Messages;
import io.crowbar.messages.VisualizationMessages;

import com.sun.net.httpserver.HttpExchange;
import java.io.IOException;
import java.io.OutputStream;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.StringTokenizer;

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
        StringTokenizer tok = new StringTokenizer(relativePath, "/");

        OutputStream os = t.getResponseBody();

        int nTok = tok.countTokens();


        t.sendResponseHeaders(200, 0);

        if (nTok == 0) {
            sendLinks(t, os);
        } else if (nTok == 1) {
            String id = tok.nextToken();
            sendLinks(t, os, id);
        }

        if (tok.countTokens() == 2) {
            String id = tok.nextToken();
            String con = tok.nextToken();
            sendReport(t, os, id, con);
        }

        os.close();

        String response = "";
    }

    public void sendLinks (HttpExchange t,
                           OutputStream os)
    throws IOException {
        for (String id : entries.keySet())
            os.write(("<a href=\"" + t.getHttpContext().getPath() + "/" + id + "\">" + id + "</a>\n").getBytes());
    }

    public void sendLinks (HttpExchange t,
                           OutputStream os,
                           String id)
    throws IOException {
        Entry e = entries.get(id);
        DiagnosticSystem ds = e.getDiagnosticSystem();


        if (ds == null)
            return;

        for (Connection c : ds.getConnections()) {
            os.write(("<a href=\"" + t.getRequestURI().getPath() + "/" + c.getId() + "\">" + c + "</a>\n").getBytes());
        }
    }

    public void sendReport (HttpExchange t,
                            OutputStream os,
                            String id,
                            String con) throws IOException {
        Entry e = entries.get(id);

        DiagnosticSystem ds = e.getDiagnosticSystem();
        Spectrum spectrum = e.getSpectrum();
        DiagnosticReport dr = e.getDiagnosticReport();
        Connection c = ds.getConnections().get(Integer.parseInt(con));
        Diagnostic diag = dr.getDiagnostic(c);


        os.write(diag.toString().getBytes());
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