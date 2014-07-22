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
            return new VerboseListener();
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