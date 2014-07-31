package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.messaging.Messages.ByeMessage;
import io.crowbar.instrumentation.messaging.Messages.Message;
import io.crowbar.instrumentation.messaging.Messages.HelloMessage;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;

import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.Queue;
import java.util.LinkedList;
import java.util.UUID;

public class Client implements EventListener {
    class Dispatcher extends Thread {
        public void run () {
            Message message = getMessage();


            while (message != null) {
                try {
                    if (s == null) {
                        s = new Socket(host, port);
                        ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
                        out.writeObject(new HelloMessage(clientId));
                        out.flush();
                    }

                    if (message != null) {
                        ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
                        // System.out.println("Sending " + message);
                        out.writeObject(message);
                        out.flush();
                    }

                    message = getMessage();
                }
                catch (Exception e) {
                    System.err.println("Exception, reseting socket");
                    e.printStackTrace();

                    s = null;
                    try {
                        Thread.sleep(10000);
                    }
                    catch (Exception e2) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    private Queue<Message> messages = new LinkedList<Message> ();
    private Socket s = null;
    private Thread t = null;
    private final String clientId = UUID.randomUUID().toString();
    private String host;
    private int port;


    public Client (String host, int port) {
        this.host = host;
        this.port = port;

        /*!
         *  This blob ensures that the connection with the server is
         *  properly closed by sending a ByeMessage when the VM shuts
         *  down.
         */
        Runtime.getRuntime().addShutdownHook(
            new Thread() {
                public void run() {
                    try {
                        postMessage(new ByeMessage());
                        t.join();
                    }
                    catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
    }

    public final String getCliendId () {
        return this.clientId;
    }

    private synchronized void postMessage (Messages.Message m) {
        messages.add(m);

        if (t == null) {
            t = new Dispatcher();
            t.start();
        }
    }

    private synchronized Message getMessage () {
        if (messages.size() == 0) {
            t = null;
            return null;
        }

        return messages.poll();
    }

    @Override
    public final void registerNode (Node node) {
        postMessage(new Messages.RegisterNodeMessage(node));
    }

    @Override
    public final void registerProbe (Probe probe) {
        postMessage(new Messages.RegisterProbeMessage(probe));
    }

    @Override
    public final void startTransaction (int probeId) {
        postMessage(new Messages.TransactionStartMessage(probeId));
    }

    @Override
    public final void endTransaction (int probeId,
                                      String exception,
                                      boolean[] hitVector) {
        postMessage(new Messages.TransactionEndMessage(probeId,
                                                       exception,
                                                       hitVector));
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {
        postMessage(new Messages.OracleMessage(probeId, error, confidence));
    }
}