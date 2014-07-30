package io.crowbar.sandbox;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.events.MultiListener;
import io.crowbar.instrumentation.events.SpectraBuilder;
import io.crowbar.instrumentation.events.TreeRebuilder;
import io.crowbar.instrumentation.events.VerboseListener;
import io.crowbar.instrumentation.messaging.Server;
import io.crowbar.instrumentation.runtime.TreeJSONSerializer;

import java.net.ServerSocket;

public final class AgentServer {
    private static class DummyService implements Server.Service {
        private MultiListener ml = new MultiListener();
        private SpectraBuilder sb = new SpectraBuilder("org\\.junit\\.Assume\\$AssumptionViolatedException"); // Ignore tests that ended with this exception
        private TreeRebuilder tr = new TreeRebuilder();

        DummyService (String id) {
            // VerboseListener vl1 = new VerboseListener();


            // vl1.prefix = "[[ " + id + " ]][[[ Pre-TreeRebuilder  ]]] ";
            // ml.add(vl1);

            ml.add(tr);

            VerboseListener vl2 = new VerboseListener();
            // vl2.prefix = "[[ " + id + " ]][[[ Post-TreeRebuilder ]]] ";
            ml.add(vl2);

            ml.add(sb);
        }

        @Override
        public EventListener getEventListener () {
            return ml;
        }

        @Override
        public void interrupted () {
            System.out.println(TreeJSONSerializer.serialize(tr));
            System.out.println(sb);
        }

        @Override
        public void finalize () {
            System.out.println(sb);
        }
    }

    public static class DummyServiceFactory implements Server.ServiceFactory {
        public final Server.Service create (String id) {
            return new DummyService(id);
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