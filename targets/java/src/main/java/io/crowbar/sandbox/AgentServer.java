package io.crowbar.sandbox;

import io.crowbar.instrumentation.events.*;
import io.crowbar.instrumentation.messaging.Server;
import io.crowbar.instrumentation.runtime.*;

import java.util.HashMap;
import java.util.Map;
import java.net.ServerSocket;

public class AgentServer {
    public static class VerboseListenerFactory implements Server.EventListenerFactory {
        public EventListener create (String id) {
            MultiListener ml = new MultiListener();

            VerboseListener vl1 = new VerboseListener();


            vl1.prefix = "[[ " + id + " ]][[[ Pre-TreeRebuilder  ]]] ";
            ml.add(vl1);

            ml.add(new TreeRebuilder());

            VerboseListener vl2 = new VerboseListener();
            vl2.prefix = "[[ " + id + " ]][[[ Post-TreeRebuilder ]]] ";
            ml.add(vl2);
            return ml;
        }
    }

    public static void main (String[] args) {
        try {
            Server s = new Server(new ServerSocket(1234),
                                  new VerboseListenerFactory());
            s.start();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}