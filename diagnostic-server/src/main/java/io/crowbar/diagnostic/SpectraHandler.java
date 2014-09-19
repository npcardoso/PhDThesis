package io.crowbar.diagnostic;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.SpectrumView;
import io.crowbar.diagnostic.spectrum.SpectrumViewFactory;
import io.crowbar.diagnostic.spectrum.matchers.SpectrumMatcher;

import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.OutputStream;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import io.crowbar.diagnostic.spectrum.serializers.HitSpectrumSerializer;
import java.io.IOException;

public final class SpectraHandler extends AbstractHttpHandler {
    public static class Entry {
        private final Spectrum original;
        private final List<SpectrumView> views;

        public Spectrum getOriginal () {
            return original;
        }

        public Spectrum getFinal () {
            if (views.size() == 0)
                return original;

            return views.get(views.size() - 1);
        }

        public List<SpectrumView> getViews () {
            return views;
        }

        public Entry (Spectrum original,
                      List<SpectrumMatcher> matchers) {
            this.original = original;
            List<SpectrumView> tmp = new ArrayList<SpectrumView> ();
            SpectrumViewFactory svf = new SpectrumViewFactory(original);

            tmp.add(svf.getView());

            for (SpectrumMatcher m : matchers) {
                svf.addStage(m);
                tmp.add(svf.getView());
            }

            this.views = Collections.unmodifiableList(tmp);
        }
    }

    private final List<SpectrumMatcher> spectrumMatchers;
    private final Map<String, Entry> entries = new HashMap<String, Entry> ();

    public SpectraHandler (List<SpectrumMatcher> spectrumMatchers) {
        this.spectrumMatchers = spectrumMatchers;
    }

    @Override
    public void handle (HttpExchange t,
                        String relativePath)
    throws Exception {
        StringTokenizer tok = new StringTokenizer(relativePath, "/");

        OutputStream os = t.getResponseBody();

        int nTok = tok.countTokens();


        if (nTok == 0) {
            sendLinks(t, os);
        } else if (nTok == 1) {
            String id = tok.nextToken();
            sendLinks(t, os, id);
        }

        if (tok.countTokens() == 2) {
            String id = tok.nextToken();
            String specId = tok.nextToken();
            sendSpectrum(t, os, id, specId);
        }

        os.close();
    }

    public void sendLinks (HttpExchange t,
                           OutputStream os)
    throws IOException {
        t.sendResponseHeaders(200, 0);

        for (String id : entries.keySet())
            os.write(("<a href=\"" + t.getHttpContext().getPath() + "/" + id + "\">" + id + "</a>\n").getBytes());
    }

    public void sendLinks (HttpExchange t,
                           OutputStream os,
                           String id)
    throws IOException {
        Entry e = entries.get(id);


        if (e == null)
            return;


        t.sendResponseHeaders(200, 0);
        os.write(("<a href=\"" + t.getRequestURI().getPath() + "/0\">original</a>\n").getBytes());
        os.write(("<a href=\"" + t.getRequestURI().getPath() + "/" + (e.getViews().size() - 1) + "\">final</a>\n").getBytes());

        for (int i = 0; i < e.getViews().size(); i++) {
            os.write(("<a href=\"" + t.getRequestURI().getPath() + "/" + i + "\">" + i + "</a>\n").getBytes());
        }
    }

    public void sendSpectrum (HttpExchange t,
                              OutputStream os,
                              String id,
                              String specId) throws IOException {
        Spectrum s = entries.get(id).getViews().get(Integer.parseInt(specId));


        if (s == null)
            return;


        t.sendResponseHeaders(200, 0);
        os.write(s.getTree().toString().getBytes());
        os.write("\n".getBytes());
        os.write(s.getProbes().toString().getBytes());
        os.write("\n".getBytes());
        os.write(HitSpectrumSerializer.serialize(s).getBytes());
    }

    public Entry handle (String id,
                         Spectrum s) {
        Entry e = new Entry(s, spectrumMatchers);


        entries.put(id, e);

        return e;
    }
}