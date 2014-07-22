package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.messaging.Messages.*;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.util.io.ThreadedServer;

import java.io.EOFException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server extends ThreadedServer {
    public interface EventListenerFactory {
        public EventListener create (String id);
    }

    public static class Service implements Runnable {
        public Service (Socket s,
                        EventListener event_listener) {
            this.s = s;
            this.event_listener = event_listener;
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

                if (o instanceof TransactionStartMessage)
                    event_listener.startTransaction(m.probe_id);
                else if (o instanceof TransactionEndMessage)
                    event_listener.endTransaction(m.probe_id,
                                                  ((TransactionEndMessage) m).hit_vector);
                else if (o instanceof OracleMessage)
                    event_listener.oracle(m.probe_id,
                                          ((OracleMessage) m).error,
                                          ((OracleMessage) m).confidence);
            }
            else if (o instanceof RegisterMessage) {
                // ProbeSet probe_set = ((RegisterMessage) o).probe_set;

                // Auto registration
                // probe_store.register(probe_set);
                // event_listener.register(probe_set);
            }
            else
                throw new Exception("Unknown Message Type: " + o);
        }

        private Socket s;
        private EventListener event_listener;
    }

    public Server (ServerSocket server_socket,
                   EventListenerFactory event_listener_factory) {
        super(server_socket);
        this.event_listener_factory = event_listener_factory;
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

        EventListener event_listener = event_listener_factory.create(id);
        return new Service(s, event_listener);
    }

    EventListenerFactory event_listener_factory;
}