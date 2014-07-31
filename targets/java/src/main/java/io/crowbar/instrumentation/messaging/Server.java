package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.messaging.Messages.HelloMessage;
import io.crowbar.instrumentation.messaging.Messages.ByeMessage;
import io.crowbar.instrumentation.messaging.Messages.ProbeMessage;
import io.crowbar.instrumentation.messaging.Messages.TransactionStartMessage;
import io.crowbar.instrumentation.messaging.Messages.TransactionEndMessage;
import io.crowbar.instrumentation.messaging.Messages.OracleMessage;
import io.crowbar.instrumentation.messaging.Messages.RegisterNodeMessage;
import io.crowbar.instrumentation.messaging.Messages.RegisterProbeMessage;
import io.crowbar.util.io.ThreadedServer;

import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server extends ThreadedServer {
    public interface Service {
        EventListener getEventListener ();
        void interrupted ();
        void finalize ();
    }

    public interface ServiceFactory {
        Service create (String id);
    }

    private class Dispatcher implements Runnable {
        private Socket socket;
        private Service service = null;
        public Dispatcher (Socket s) {
            socket = s;
        }

        @Override
        public final void run () {
            try {
                String id = handshake();
                service = serviceFactory.create(id);

                while (true) {
                    Object o = new ObjectInputStream(socket.getInputStream()).readObject();

                    if (o instanceof ByeMessage) {
                        service.finalize();
                        break;
                    }
                    else
                        dispatch(o);
                }
            }
            catch (Throwable e) {
                if (service != null)
                    service.interrupted();

                e.printStackTrace();
            }
            try {
                socket.close();
            }
            catch (Throwable e) {}
        }

        /*!
         * Performs the handshake and returns client id on sucess.
         */
        private String handshake () throws Exception {
            Object o = new ObjectInputStream(socket.getInputStream()).readObject();


            // System.out.println("Receiving " + o);

            if (!(o instanceof HelloMessage))
                throw new Exception("First message should be a HelloMessage. Received instead: " + o);

            return ((HelloMessage) o).getId();
        }

        private void dispatch (Object o) {
            EventListener eventListener = service.getEventListener();


            try {
                if (o instanceof ProbeMessage) {
                    ProbeMessage m = (ProbeMessage) o;

                    if (o instanceof TransactionStartMessage)
                        eventListener.startTransaction(m.getProbeId());
                    else if (o instanceof TransactionEndMessage)
                        eventListener.endTransaction(m.getProbeId(),
                                                     ((TransactionEndMessage) m).getException(),
                                                     ((TransactionEndMessage) m).getHitVector());
                    else if (o instanceof OracleMessage)
                        eventListener.oracle(m.getProbeId(),
                                             ((OracleMessage) m).getError(),
                                             ((OracleMessage) m).getConfidence());
                }
                else if (o instanceof RegisterNodeMessage) {
                    eventListener.registerNode(((RegisterNodeMessage) o).getNode());
                }
                else if (o instanceof RegisterProbeMessage) {
                    RegisterProbeMessage m = (RegisterProbeMessage) o;
                    eventListener.registerProbe(m.getProbe());
                }
                else {
                    throw new Exception("Unknown Message Type: " + o);
                }
            }
            catch (Throwable e) {
                e.printStackTrace();
            }
        }
    }

    public Server (ServerSocket serverSocket,
                   ServiceFactory serviceFactory) {
        super(serverSocket);
        this.serviceFactory = serviceFactory;
    }

    @Override
    protected final Runnable handle (Socket s) {
        return new Dispatcher(s);
    }

    private ServiceFactory serviceFactory;
}