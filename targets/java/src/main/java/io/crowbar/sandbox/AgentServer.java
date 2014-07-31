package io.crowbar.sandbox;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.events.SpectraBuilder;
import io.crowbar.instrumentation.messaging.Server;
import io.crowbar.instrumentation.runtime.TreeJSONSerializer;
import io.crowbar.instrumentation.spectra.HitSpectraSerializer;

import java.net.ServerSocket;

public final class AgentServer {
    private static class DummyService implements Server.Service {
        private SpectraBuilder spectraBuilder =
            new SpectraBuilder();

        @Override
        public EventListener getEventListener () {
            return spectraBuilder;
        }

        private void showState () {
            System.out.println("-------------------JSON-----------------------");
            System.out.println(TreeJSONSerializer.serialize(spectraBuilder.getTree()));
            System.out.println("-------------------HIT SPECTRA-----------------------");
            System.out.println(HitSpectraSerializer.serialize(spectraBuilder.getSpectra()));
        }

        @Override
        public void interrupted () {
            System.out.println("------------------- Interrupted -----------------------");
            showState();
            System.out.println("------------------- Interrupted -----------------------");
        }

        @Override
        public void finalize () {
            System.out.println("------------------- finalizing -----------------------");
            showState();
            System.out.println("------------------- finalizing -----------------------");
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