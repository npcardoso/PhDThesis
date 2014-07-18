package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.messaging.Messages.*;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.CollectorListener;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.util.io.ThreadedServer;

import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Server extends ThreadedServer {
    BlockingQueue<Message> messages = new LinkedBlockingQueue<Message> ();

    public static abstract class ServiceFactory {
        public abstract Service create (Socket s);
    }

    public static abstract class Service implements Runnable {
        protected void handle (TransactionStartMessage m) throws Exception {}
        protected void handle (TransactionEndMessage m) throws Exception {}
        protected void handle (OracleMessage m) throws Exception {}
        protected void handle (RegisterMessage m) throws Exception {}

        public Service (Socket s) {
            this.s = s;
        }

        @Override
        public final void run () {
            while (true) {
                Object m;
                try {
                    ObjectInputStream in = new ObjectInputStream(s.getInputStream());
                    m = in.readObject();
                }
                catch (Exception e) {
                    e.printStackTrace();
                    break;
                }

                try {
                    dispatch(m);
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        private void dispatch (Object m) throws Exception {
            if (m instanceof TransactionStartMessage)
                handle((TransactionStartMessage) m);
            else if (m instanceof TransactionEndMessage)
                handle((TransactionEndMessage) m);
            else if (m instanceof OracleMessage)
                handle((OracleMessage) m);
            else if (m instanceof RegisterMessage)
                handle((RegisterMessage) m);
            else
                throw new Exception("Unknown Message Type: " + m);
        }

        private Socket s;
    }

    public Server (ServiceFactory service_factory,
                   ServerSocket server_socket) {
        super(server_socket);
        this.service_factory = service_factory;
    }

    @Override
    protected Runnable handle (Socket s) {
        return service_factory.create(s);
    }

    ServiceFactory service_factory;
}