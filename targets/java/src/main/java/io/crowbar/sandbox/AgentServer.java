package io.crowbar.sandbox;

import io.crowbar.instrumentation.events.*;
import io.crowbar.instrumentation.messaging.Server;
import io.crowbar.instrumentation.runtime.*;

import java.util.HashMap;
import java.util.Map;
import java.net.ServerSocket;

public class AgentServer {
    public static class DummyService implements Server.Service {
        MultiListener ml = new MultiListener();
        SpectraBuilder sb = new SpectraBuilder();
        TreeRebuilder tr = new TreeRebuilder();
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
        public Server.Service create (String id) {
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