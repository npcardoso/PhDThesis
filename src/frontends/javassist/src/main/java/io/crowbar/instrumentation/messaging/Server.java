package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.messaging.Messages.*;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.ProbeSet;
import io.crowbar.instrumentation.runtime.ProbeStore;
import io.crowbar.util.io.ThreadedServer;

import java.io.EOFException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server extends ThreadedServer {
    public interface EventListenerFactory {
        public EventListener create (ProbeStore ps);
    }

    public interface ProbeStoreFactory {
        public ProbeStore getOrCreate (String id);
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
            else if (o instanceof RegisterMessage) {
                ProbeSet probe_set = ((RegisterMessage) o).probe_set;

                // Auto registration
                probe_store.register(probe_set);
                event_listener.register(probe_set);
            }
            else
                throw new Exception("Unknown Message Type: " + o);
        }

        private Socket s;
        private EventListener event_listener;
        private ProbeStore probe_store;
    }

    public Server (ServerSocket server_socket,
                   EventListenerFactory event_listener_factory,
                   ProbeStoreFactory probe_store_factory) {
        super(server_socket);
        this.event_listener_factory = event_listener_factory;
        this.probe_store_factory = probe_store_factory;
    }

    @Override
    protected Runnable handle (Socket s) {
        String id = null;


        try {
            Object o;
            ObjectInputStream in = new ObjectInputStream(s.getInputStream());
            o = in.readObject();

            if (!(o instanceof HelloMessage))
                throw new Exception("First message should be a HelloMessage. Received instead: " + o);

            id = ((HelloMessage) o).id;
        }
        catch (Exception e) {
            e.printStackTrace();
            return null;
        }

        ProbeStore probe_store = probe_store_factory.getOrCreate(id);
        EventListener event_listener = event_listener_factory.create(probe_store);
        return new Service(s,
                           event_listener,
                           probe_store);
    }

    EventListenerFactory event_listener_factory;
    ProbeStoreFactory probe_store_factory;
}