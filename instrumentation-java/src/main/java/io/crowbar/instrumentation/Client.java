package io.crowbar.instrumentation;

import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.Messages.ByeMessage;
import io.crowbar.instrumentation.Messages.HelloMessage;
import io.crowbar.instrumentation.Messages.Message;
import io.crowbar.instrumentation.events.EventListener;

import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.LinkedList;
import java.util.Queue;
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
                        postMessage(new ByeMessage()).join();
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

    /*!
     * Returns posts message and returns dispatcher thread.
     */
    private synchronized Thread postMessage (Messages.Message m) {
        messages.add(m);

        if (t == null) {
            t = new Dispatcher();
            t.start();
        }

        return t;
    }

    private synchronized Message getMessage () {
        if (messages.size() == 0) {
            t = null;
            return null;
        }

        return messages.poll();
    }

    @Override
    public void registerNode (int nodeId,
                              int parentId,
                              String name) throws Exception {
        postMessage(new Messages.RegisterNodeMessage(nodeId,
                                                     parentId,
                                                     name));
    }

    @Override
    public void registerProbe (int probeId,
                               int nodeId,
                               ProbeType type) throws Exception {
        postMessage(new Messages.RegisterProbeMessage(probeId,
                                                      nodeId,
                                                      type));
    }

    @Override
    public final void startTransaction (int probeId) {
        postMessage(new Messages.TransactionStartMessage(probeId));
    }

    @Override
    public final void endTransaction (int probeId,
                                      String exceptionClass,
                                      String exceptionMessage,
                                      boolean[] hitVector) {
        postMessage(new Messages.TransactionEndMessage(probeId,
                                                       exceptionClass,
                                                       exceptionMessage,
                                                       hitVector));
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {
        postMessage(new Messages.OracleMessage(probeId, error, confidence));
    }
}
