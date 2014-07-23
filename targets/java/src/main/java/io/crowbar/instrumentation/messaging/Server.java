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
    public interface Service {
        public EventListener getEventListener ();
        public void interrupted ();
        public void finalize ();
    }

    public interface ServiceFactory {
        public Service create (String id);
    }

    private class Dispatcher implements Runnable {
        private Socket socket;
        private Service service = null;
        public Dispatcher (Socket s) {
            socket = s;
        }

        @Override
        public final void run () {
            String id = null;
            Object o = null;


            try {
                o = new ObjectInputStream(socket.getInputStream()).readObject();
                System.out.println("Receiving " + o);

                if (!(o instanceof HelloMessage))
                    throw new Exception("First message should be a HelloMessage. Received instead: " + o);

                id = ((HelloMessage) o).id;
            }
            catch (Exception e) {
                e.printStackTrace();
                return;
            }

            service = serviceFactory.create(id);

            while (true) {
                try {
                    o = new ObjectInputStream(socket.getInputStream()).readObject();
                    System.out.println("Receiving " + o);
                }
                catch (Exception e) {
                    service.interrupted();
                    e.printStackTrace();
                    return;
                }

                if (o instanceof ByeMessage) {
                    service.finalize();
                    return;
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
            EventListener event_listener = service.getEventListener();


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
            else if (o instanceof RegisterNodeMessage) {
                event_listener.registerNode(((RegisterNodeMessage) o).node);
            }
            else if (o instanceof RegisterProbeMessage) {
                RegisterProbeMessage m = (RegisterProbeMessage) o;
                event_listener.registerProbe(m.probe_id,
                                             m.node_id,
                                             m.type);
            }
            else
                throw new Exception("Unknown Message Type: " + o);
        }
    }

    public Server (ServerSocket serverSocket,
                   ServiceFactory serviceFactory) {
        super(serverSocket);
        this.serviceFactory = serviceFactory;
    }

    @Override
    protected Runnable handle (Socket s) {
        return new Dispatcher(s);
    }

    ServiceFactory serviceFactory;
}