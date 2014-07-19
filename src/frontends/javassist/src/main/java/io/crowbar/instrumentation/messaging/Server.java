package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.messaging.Messages.*;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.ProbeStore;
import io.crowbar.util.io.ThreadedServer;

import java.io.EOFException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server extends ThreadedServer {
    public static abstract class EventListenerFactory {
        public abstract EventListener create ();
    }

    public static class Service implements Runnable {
        public Service (Socket s,
                        EventListener event_listener,
                        ProbeStore probe_store) {
            this.s = s;
            this.event_listener = event_listener;
            this.probe_store = probe_store;
        }

        @Override
        public final void run () {
            while (true) {
                Object o;
                try {
                    ObjectInputStream in = new ObjectInputStream(s.getInputStream());
                    o = in.readObject();
                }
                catch (EOFException e) {
                    break;
                }
                catch (Exception e) {
                    e.printStackTrace();
                    break;
                }

                try {
                    dispatch(o);
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        private void dispatch (Object o) throws Exception {
            if (o instanceof ProbeMessage) {
                ProbeMessage m = (ProbeMessage) o;
                Probe p = probe_store.get(m.probe_set_id, m.probe_id);

                if (o instanceof TransactionStartMessage)
                    event_listener.startTransaction(p);
                else if (o instanceof TransactionEndMessage)
                    event_listener.endTransaction(p, ((TransactionEndMessage) m).hit_vector);
                else if (o instanceof OracleMessage)
                    event_listener.oracle(p, ((OracleMessage) m).error, ((OracleMessage) m).confidence);
            }
            else if (o instanceof RegisterMessage)
                event_listener.register(((RegisterMessage) o).probe_set);
            else
                throw new Exception("Unknown Message Type: " + o);
        }

        private Socket s;
        private EventListener event_listener;
        private ProbeStore probe_store;
    }

    public Server (ServerSocket server_socket,
                   EventListenerFactory event_listener_factory,
                   ProbeStore probe_store) {
        super(server_socket);
        this.event_listener_factory = event_listener_factory;
        this.probe_store = probe_store;
    }

    @Override
    protected Runnable handle (Socket s) {
        return new Service(s, event_listener_factory.create(), probe_store);
    }

    EventListenerFactory event_listener_factory;
    ProbeStore probe_store;
}