package io.crowbar.diagnostic;


import io.crowbar.diagnostic.algorithms.SimilarityRanker;
import io.crowbar.diagnostic.algorithms.SingleFaultGenerator;
import io.crowbar.diagnostic.database.DiagnosticEntry;
import io.crowbar.diagnostic.database.SpectrumEntry;
import io.crowbar.diagnostic.database.Database;
import io.crowbar.diagnostic.handlers.DiagnosticReportHandler;
import io.crowbar.diagnostic.handlers.SpectraHandler;
import io.crowbar.diagnostic.handlers.StaticContentHttpHandler;
import io.crowbar.diagnostic.handlers.StaticLinksHandler;
import io.crowbar.diagnostic.spectrum.ProbeType;
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
import io.crowbar.util.JSonUtils;


import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.net.ServerSocket;

import com.sun.net.httpserver.HttpServer;
import java.net.URI;
import javax.ws.rs.Path;
import javax.ws.rs.GET;
import javax.ws.rs.Produces;
import org.glassfish.jersey.filter.LoggingFilter;
import org.glassfish.jersey.jdkhttp.JdkHttpServerFactory;
import org.glassfish.jersey.server.ResourceConfig;


public final class DiagnosticServer {
    private class InstrumentationServiceFactory
    implements InstrumentationServer.ServiceFactory {
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
                SpectrumEntry e = new SpectrumEntry(spectrumBuilder.getSpectrum(), specMatchers);


                db.handle(id, e);
                try {
                    JNARunner runner = new JNARunner();
                    System.out.println(">>>>>> Inside JNA >>>>>>");
                    DiagnosticReport dr = runner.run(diagSystem, e.getFinal());
                    System.out.println("<<<<<< Inside JNA <<<<<<");
                    db.handle(id, new DiagnosticEntry(diagSystem, e.getFinal(), dr));
                }
                catch (Throwable ex) {
                    ex.printStackTrace();
                }
            }
        }

        public final InstrumentationServer.Service create (String id) {
            return new InstrumentationService(id);
        }
    }


    private final Database db = new Database();

    private final DiagnosticSystem diagSystem;
    private final List<SpectrumMatcher> specMatchers;

    private final InstrumentationServer instrServer;
    private final HttpServer httpServer;

    public DiagnosticServer (DiagnosticSystem ds,
                             List<SpectrumMatcher> spectrumMatchers,
                             int instrPort,
                             int httpPort) throws Exception {
        diagSystem = ds;
        specMatchers = spectrumMatchers;
        instrServer = new InstrumentationServer(new ServerSocket(instrPort),
                                                new InstrumentationServiceFactory());


        StaticLinksHandler staticLinks = new StaticLinksHandler();
        staticLinks.addLink("<div><a href=/spectra/>Spectra</a></div>\n");
        staticLinks.addLink("<div><a href=/dr/>DiagnosticReport</a></div>\n");
        staticLinks.addLink("<div><a href=/visualizations/index.html>Visualizations</a></div>\n");

        URI endpoint = new URI("http://localhost:" + httpPort + "/");
        ResourceConfig rc = new ResourceConfig();
        rc.registerInstances(new SpectraHandler(db, JSonUtils.getPrettySerializer()));
        rc.registerInstances(new DiagnosticReportHandler(db, JSonUtils.getPrettySerializer()));
        rc.registerInstances(staticLinks);
        rc.register(new LoggingFilter());

        httpServer = JdkHttpServerFactory.createHttpServer(endpoint, rc, false);


        httpServer.createContext("/visualizations/", new StaticContentHttpHandler("../visualizations/src"));

        httpServer.setExecutor(null); // creates a default executor
    }

    public void start () {
        instrServer.start();
        httpServer.start();
    }

    public static void main (String[] args) {
        try {
            DiagnosticSystemFactory j = new DiagnosticSystemFactory();
            j.addGenerator(new SingleFaultGenerator());
            j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
            j.addConnection(0, 0);

            List<SpectrumMatcher> specMatchers = new ArrayList<SpectrumMatcher> ();
            specMatchers.add(new NegateMatcher(new JUnitAssumeMatcher(false)));
            specMatchers.add(new NegateMatcher(new TestProbesMatcher()));
            specMatchers.add(new ProbeTypeMatcher(ProbeType.HIT_PROBE));
            specMatchers.add(new ActiveProbeMatcher());
            specMatchers.add(new SuspiciousProbeMatcher());
            specMatchers.add(new ValidTransactionMatcher());


            DiagnosticServer diagServer = new DiagnosticServer(j.create(),
                                                               specMatchers,
                                                               1234,
                                                               8080);
            diagServer.start();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}