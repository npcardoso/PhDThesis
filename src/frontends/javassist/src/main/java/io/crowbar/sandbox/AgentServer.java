package io.crowbar.sandbox;

import io.crowbar.instrumentation.events.*;
import io.crowbar.instrumentation.messaging.Server;
import io.crowbar.instrumentation.runtime.*;

import java.util.HashMap;
import java.util.Map;
import java.net.ServerSocket;

public class AgentServer {
    public static class ProbeStoreFactoryFoo implements Server.ProbeStoreFactory {
        public ProbeStore getOrCreate (String id) {
            ProbeStore probe_store = probe_stores.get(id);


            if (probe_store == null) {
                probe_store = new ProbeStore();
                probe_stores.put(id, probe_store);
            }

            return probe_store;
        }

        Map<String, ProbeStore> probe_stores = new HashMap<String, ProbeStore> ();
    }


    public static class VerboseListenerFactory implements Server.EventListenerFactory {
        public EventListener create (ProbeStore probe_store) {
            return new VerboseListener();
        }
    }

    public static void main (String[] args) {
        try {
            Server s = new Server(new ServerSocket(1234),
                                  new VerboseListenerFactory(),
                                  new ProbeStoreFactoryFoo());

            s.start();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}