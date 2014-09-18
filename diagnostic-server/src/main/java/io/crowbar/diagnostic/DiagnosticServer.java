package io.crowbar.diagnostic;

import io.crowbar.diagnostic.algorithms.SimilarityRanker;
import io.crowbar.diagnostic.algorithms.SingleFaultGenerator;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.SpectrumViewFactory;
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


import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;


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
            handle(id, spectrumBuilder.getSpectrum());
        }
    }

    private class InstrumentationServiceFactory
    implements InstrumentationServer.ServiceFactory {
        public final InstrumentationServer.Service create (String id) {
            return new InstrumentationService(id);
        }
    }


    public void handle (String id,
                        Spectrum s) {
        SpectrumViewFactory svf = new SpectrumViewFactory(s);


        svf.addStage(new ProbeTypeMatcher(ProbeType.HIT_PROBE));
        svf.addStage(new ActiveProbeMatcher());
        svf.addStage(new SuspiciousProbeMatcher());
        svf.addStage(new NegateMatcher(new JUnitAssumeMatcher(false)));
        svf.addStage(new TestProbesMatcher());
        svf.addStage(new ValidTransactionMatcher());


        DiagnosticSystemFactory j = new DiagnosticSystemFactory();
        j.addGenerator(new SingleFaultGenerator());
        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        j.addConnection(0, 0);

        DiagnosticSystem ds = j.create();
        Spectrum spectrum = svf.getView();


        try {
            JNARunner runner = new JNARunner();

            System.out.println("------ Inside JNA --------");
            DiagnosticReport dr = runner.run(ds, spectrum);
            System.out.println("------ Inside JNA --------");

            handle(id, dr);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    public void handle (String id,
                        DiagnosticReport dr) {
        /*
         * Diagnostic diag = dr.getDiagnostic(fuzzinelCon);
         * List<Double> scores = spectrum.getScorePerNode(diag, Spectrum.SUM);
         * Tree t = spectrumBuilder.getSpectrum().getTree();
         *
         * String jsonRequest = io.crowbar.messages.Messages.serialize(
         * VisualizationMessages.issueRequest(t,scores));
         * System.out.println(jsonRequest);
         */
    }

    static class MyHandler implements HttpHandler {
        public void handle (HttpExchange t) throws IOException {
            String response = "This is the response";


            t.sendResponseHeaders(200, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
        }
    }


    private final InstrumentationServer instrServer;
    private final HttpServer httpServer;

    public DiagnosticServer (ServerSocket instrServerSocket,
                             int httpPort) throws IOException {
        instrServer = new InstrumentationServer(instrServerSocket,
                                                new InstrumentationServiceFactory());

        httpServer = HttpServer.create(new InetSocketAddress(httpPort), 0);

        httpServer.createContext("/test", new MyHandler());
        httpServer.setExecutor(null); // creates a default executor
    }

    public void start () {
        httpServer.start();
        instrServer.start();
    }

    public static void main (String[] args) {
        try {
            DiagnosticServer diagServer = new DiagnosticServer(new ServerSocket(1234), 8080);
            diagServer.start();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}