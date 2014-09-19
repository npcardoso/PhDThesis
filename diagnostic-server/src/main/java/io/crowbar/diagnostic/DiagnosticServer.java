package io.crowbar.diagnostic;

import io.crowbar.diagnostic.algorithms.SimilarityRanker;
import io.crowbar.diagnostic.algorithms.SingleFaultGenerator;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.SpectrumViewFactory;
import io.crowbar.diagnostic.spectrum.matchers.SpectrumMatcher;
import io.crowbar.diagnostic.spectrum.matchers.ActiveProbeMatcher;
import io.crowbar.diagnostic.spectrum.matchers.NegateMatcher;
import io.crowbar.diagnostic.spectrum.matchers.ProbeTypeMatcher;
import io.crowbar.diagnostic.spectrum.matchers.SuspiciousProbeMatcher;
import io.crowbar.diagnostic.spectrum.matchers.TestProbesMatcher;
import io.crowbar.diagnostic.spectrum.matchers.ValidTransactionMatcher;
import io.crowbar.diagnostic.runners.JNARunner;
import io.crowbar.instrumentation.InstrumentationServer;
import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.spectrum.SpectrumBuilder;
import io.crowbar.instrumentation.spectrum.matcher.JUnitAssumeMatcher;
import io.crowbar.messages.VisualizationMessages;


import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import io.crowbar.diagnostic.spectrum.Tree;
import java.util.ArrayList;
import java.util.LinkedList;


class DiagnosticServer {
    private class InstrumentationService
    implements InstrumentationServer.Service {
        private final String id;
        private final SpectrumBuilder spectrumBuilder =
            new SpectrumBuilder();

        InstrumentationService (String id) {
            this.id = id;
        }

        @Override
        public EventListener getEventListener () {
            return spectrumBuilder;
        }

        @Override
        public void interrupted () {}

        @Override
        public void terminate () {
            SpectraHandler.Entry specRes = specHandler.handle(id, spectrumBuilder.getSpectrum());
            try {
                JNARunner runner = new JNARunner();
                System.out.println("------ Inside JNA --------");
                DiagnosticReport dr = runner.run(diagSystem, specRes.getFinal());
                System.out.println("------ Inside JNA --------");
                diagReportHandler.handle(id, diagSystem, specRes.getFinal(), dr);
            }
            catch (Throwable e) {
                e.printStackTrace();
            }
        }
    }

    private class InstrumentationServiceFactory
    implements InstrumentationServer.ServiceFactory {
        public final InstrumentationServer.Service create (String id) {
            return new InstrumentationService(id);
        }
    }

    private class StaticLinksHandler implements HttpHandler {
        private List<String> links = new LinkedList<String> ();

        public void addLink (String link) {
            links.add(link);
        }

        public final void handle (HttpExchange t) throws IOException {
            t.sendResponseHeaders(200, 0);

            OutputStream os = t.getResponseBody();

            for (String l : links) {
                os.write(l.getBytes());
            }

            os.close();
        }
    }


    private final Map<String, DiagnosticReport> reports = new HashMap<String, DiagnosticReport> ();

    private final DiagnosticSystem diagSystem;
    private final InstrumentationServer instrServer;
    private final HttpServer httpServer;

    private final SpectraHandler specHandler;
    private final DiagnosticReportHandler diagReportHandler;

    public DiagnosticServer (DiagnosticSystem ds,
                             int instrPort,
                             int httpPort) throws IOException {
        diagSystem = ds;

        instrServer = new InstrumentationServer(new ServerSocket(1234),
                                                new InstrumentationServiceFactory());

        httpServer = HttpServer.create(new InetSocketAddress(httpPort), 0);

        List<SpectrumMatcher> spectrumMatchers = new ArrayList<SpectrumMatcher> ();
        spectrumMatchers.add(new NegateMatcher(new TestProbesMatcher()));
        spectrumMatchers.add(new ProbeTypeMatcher(ProbeType.HIT_PROBE));
        spectrumMatchers.add(new ActiveProbeMatcher());
        spectrumMatchers.add(new SuspiciousProbeMatcher());
        spectrumMatchers.add(new NegateMatcher(new JUnitAssumeMatcher(false)));
        spectrumMatchers.add(new ValidTransactionMatcher());

        specHandler = new SpectraHandler(spectrumMatchers);

        diagReportHandler = new DiagnosticReportHandler();


        StaticLinksHandler staticLinks = new StaticLinksHandler();
        staticLinks.addLink("<a href=/spectra/>Spectra</a>\n");
        staticLinks.addLink("<a href=/dr/>DiagnosticReport</a>\n");
        staticLinks.addLink("<a href=/visualizations/>Visualizations</a>\n");

        httpServer.createContext("/", staticLinks);
        httpServer.createContext("/spectra/", specHandler);
        httpServer.createContext("/dr/", diagReportHandler);
        httpServer.createContext("/visualizations/", new StaticContentHttpHandler("../visualizations/src"));
        httpServer.setExecutor(null); // creates a default executor
    }

    public void start () {
        httpServer.start();
        instrServer.start();
    }

    public static void main (String[] args) {
        try {
            DiagnosticSystemFactory j = new DiagnosticSystemFactory();
            j.addGenerator(new SingleFaultGenerator());
            j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
            j.addConnection(0, 0);

            DiagnosticServer diagServer = new DiagnosticServer(j.create(),
                                                               1234,
                                                               8080);
            diagServer.start();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}