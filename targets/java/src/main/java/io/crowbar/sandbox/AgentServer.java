package io.crowbar.sandbox;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.events.MultiListener;
import io.crowbar.instrumentation.events.SpectraBuilder;
import io.crowbar.instrumentation.events.TreeRebuilder;
import io.crowbar.instrumentation.events.VerboseListener;
import io.crowbar.instrumentation.messaging.Server;
import io.crowbar.instrumentation.runtime.TreeJSONSerializer;
import io.crowbar.diagnosis.DiagnosisSession;

import java.net.ServerSocket;

public final class AgentServer {
    private static class DummyService implements Server.Service {
        private DiagnosisSession session = new DiagnosisSession(
            // Ignore tests that ended with this exception);
            new SpectraBuilder("org\\.junit\\.Assume\\$AssumptionViolatedException"),
            new TreeRebuilder());

        @Override
        public EventListener getEventListener () {
            return session;
        }

        @Override
        public void interrupted () {
            System.out.println(TreeJSONSerializer.serialize(session.getTree()));
            System.out.println(session.getSpectra());
        }

        @Override
        public void finalize () {
            System.out.println(session.getSpectra());
        }
    }

    public static class DummyServiceFactory implements Server.ServiceFactory {
        public final Server.Service create (String id) {
            return new DummyService();
        }
    }

    public static void main (String[] args) {
        try {
            Server s = new Server(new ServerSocket(1234),
                                  new DummyServiceFactory());
            s.start();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}