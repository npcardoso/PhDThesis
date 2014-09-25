package io.crowbar.rest;


import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.DiagnosticSystemFactory;
import io.crowbar.diagnostic.algorithms.SimilarityRanker;
import io.crowbar.diagnostic.algorithms.SingleFaultGenerator;
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
import io.crowbar.rest.handlers.ApiResponseHandler;
import io.crowbar.rest.handlers.DiagnosticReportHandler;
import io.crowbar.rest.handlers.ExceptionHandler;
import io.crowbar.rest.handlers.SessionHandler;
import io.crowbar.rest.handlers.SpectraHandler;
import io.crowbar.rest.handlers.StaticContentHttpHandler;
import io.crowbar.rest.handlers.StaticLinksHandler;
import io.crowbar.rest.handlers.SwaggerHandler;
import io.crowbar.rest.database.Database;
import io.crowbar.rest.database.DiagnosticEntry;
import io.crowbar.rest.database.SpectrumEntry;
import io.crowbar.util.JSonUtils;


import java.util.ArrayList;
import java.util.List;
import java.net.ServerSocket;

import com.sun.net.httpserver.HttpServer;
import java.net.URI;
import org.glassfish.jersey.filter.LoggingFilter;
import org.glassfish.jersey.jdkhttp.JdkHttpServerFactory;
import org.glassfish.jersey.server.ResourceConfig;
import java.util.Date;
import flexjson.transformer.DateTransformer;
import flexjson.JSONSerializer;


public final class DiagnosticServer {
    private class InstrumentationServiceFactory
    implements InstrumentationServer.ServiceFactory {
        private class InstrumentationService
        implements InstrumentationServer.Service {
            private final String serviceId;
            private final int sessionId;

            private final SpectrumBuilder spectrumBuilder =
                new SpectrumBuilder();

            InstrumentationService (String serviceId) {
                this.serviceId = serviceId;
                this.sessionId = db.newSession(serviceId);
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


                db.handle(sessionId, e);
                try {
                    JNARunner runner = new JNARunner();
                    System.out.println(">>>>>> Inside JNA >>>>>>");
                    DiagnosticReport dr = runner.run(diagSystem, e.getFinal());
                    System.out.println("<<<<<< Inside JNA <<<<<<");
                    db.handle(sessionId, new DiagnosticEntry(diagSystem, e.getFinal(), dr));
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
        JSONSerializer json = JSonUtils.getPrettySerializer()
                              .transform(new DateTransformer("yyyy/MM/dd hh:mm:ss"), Date.class);


        diagSystem = ds;
        specMatchers = spectrumMatchers;
        instrServer = new InstrumentationServer(new ServerSocket(instrPort),
                                                new InstrumentationServiceFactory());


        StaticLinksHandler staticLinks = new StaticLinksHandler();
        staticLinks.addLink("<div><a href=/spectra/>Spectra</a></div>\n");
        staticLinks.addLink("<div><a href=/reports/>DiagnosticReport</a></div>\n");
        staticLinks.addLink("<div><a href=/visualizations/index.html>Visualizations</a></div>\n");
        staticLinks.addLink("<div><a href=api-docs/index.html?url=/service.json>Swagger</a></div>\n");

        URI endpoint = new URI("http://localhost:" + httpPort + "/");
        ResourceConfig rc = new ResourceConfig();
        rc.registerInstances(new SpectraHandler(db));
        rc.registerInstances(new DiagnosticReportHandler(db));
        rc.registerInstances(new SessionHandler(db));
        rc.registerInstances(new SwaggerHandler("target/swagger-ui/"));
        rc.registerInstances(staticLinks);
        rc.register(new LoggingFilter());
        rc.register(new ExceptionHandler(json));
        rc.register(new ApiResponseHandler(json));


        httpServer = JdkHttpServerFactory.createHttpServer(endpoint, rc, false);


        httpServer.createContext("/visualizations/", new StaticContentHttpHandler("../visualizations/src"));
        httpServer.createContext("/api-docs/", new StaticContentHttpHandler("src/main/resources/swagger-ui/dist"));

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