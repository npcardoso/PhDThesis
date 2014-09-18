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
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.messages.VisualizationMessages;


class DiagnosticServer {
    private static final DiagnosticSystem DS;

    static {
        DiagnosticSystemFactory j = new DiagnosticSystemFactory();
        j.addGenerator(new SingleFaultGenerator());
        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        j.addConnection(0, 0);

        DS = j.create();
    }

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
        spectra.put(id, s);

        SpectrumViewFactory svf = new SpectrumViewFactory(s);


        svf.addStage(new ProbeTypeMatcher(ProbeType.HIT_PROBE));
        svf.addStage(new ActiveProbeMatcher());
        svf.addStage(new SuspiciousProbeMatcher());
        svf.addStage(new NegateMatcher(new JUnitAssumeMatcher(false)));
        svf.addStage(new TestProbesMatcher());
        svf.addStage(new ValidTransactionMatcher());


        Spectrum spectrum = svf.getView();


        try {
            JNARunner runner = new JNARunner();

            System.out.println("------ Inside JNA --------");
            DiagnosticReport dr = runner.run(DS, spectrum);
            System.out.println("------ Inside JNA --------");

            handle(id, dr);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    public void handle (String id,
                        DiagnosticReport dr) {
        reports.put(id, dr);
        System.out.println("reports: " + reports);

        System.out.println("http://127.0.0.1:8080/json/" + id + "/0");
    }

    private final Map<String, Spectrum> spectra = new HashMap<String, Spectrum> ();
    private final Map<String, DiagnosticReport> reports = new HashMap<String, DiagnosticReport> ();
    class JSonHttpHandler implements HttpHandler {
        public void handle (HttpExchange t) throws IOException {
            String contextPath = t.getHttpContext().getPath();
            String requestPath = t.getRequestURI().getPath();

            String relativePath = requestPath.substring(contextPath.length() - 1);

            String response = "";


            System.out.println("relativePath: '" + relativePath + "'");
            try {
                if (relativePath.equals("/")) {
                    for (String id : reports.keySet())
                        response += id + "<br/>";
                } else {
                    Pattern p = Pattern.compile("/([^/]*)/([0-9]*)");
                    Matcher m = p.matcher(relativePath);
                    m.matches();
                    String id = m.group(1);
                    int connection = Integer.parseInt(m.group(2));


                    Spectrum spectrum = spectra.get(id);
                    Connection c = DS.getConnections().get(connection);
                    DiagnosticReport dr = reports.get(id);
                    System.out.println("id: " + id);
                    System.out.println("Connection: " + c);
                    System.out.println("spectrum: " + spectrum);
                    System.out.println("dr: " + dr);

                    Diagnostic diag = dr.getDiagnostic(c);
                    List<Double> scores = spectrum.getScorePerNode(diag, Spectrum.SUM);
                    Tree tr = spectrum.getTree();

                    response = io.crowbar.messages.Messages.serialize(
                        VisualizationMessages.issueRequest(tr, scores));
                }
            }
            catch (Throwable e) {
                e.printStackTrace();
            }
            System.out.println("Response:" + response);

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

        httpServer.createContext("/json/", new JSonHttpHandler());
        httpServer.createContext("/visualizations/", new StaticContentHttpHandler("../visualizations/src"));
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