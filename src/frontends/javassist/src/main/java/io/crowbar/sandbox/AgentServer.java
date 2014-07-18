package io.crowbar.sandbox;

import io.crowbar.instrumentation.events.*;
import io.crowbar.instrumentation.messaging.Server;
import io.crowbar.instrumentation.runtime.*;

import java.net.ServerSocket;

public class AgentServer {
    public static class VerboseListenerFactory extends Server.EventListenerFactory {
        VerboseListenerFactory (ProbeStore probe_store) {
            this.probe_store = probe_store;
        }

        public EventListener create () {
            return new VerboseListenerFoo();
        }

        public class VerboseListenerFoo extends VerboseListener implements EventListener {
            @Override
            public void register (ProbeSet ps) {
                try {
                    probe_store.register(ps);
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
                super.register(ps);
            }
        }

        private ProbeStore probe_store;
    }

    public static void main (String[] args) {
        try {
            ProbeStore probe_store = new ProbeStore();

            Server s = new Server(new ServerSocket(1234),
                                  new VerboseListenerFactory(probe_store),
                                  probe_store);

            s.start();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}