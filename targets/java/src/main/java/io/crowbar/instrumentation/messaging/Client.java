package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.messaging.Messages.Message;
import io.crowbar.instrumentation.messaging.Messages.HelloMessage;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

import java.io.ObjectOutputStream;
import java.net.ServerSocket;
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
                        out.writeObject(new HelloMessage(client_id));
                        out.flush();
                    }

                    System.out.println(message);

                    if (message != null) {
                        ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
                        out.writeObject(message);
                        out.flush();
                        System.out.println("Sending " + message);
                    }

                    message = getMessage();
                }
                catch (Exception e) {
                    System.out.println("Exception, reseting socket");
                    e.printStackTrace();

                    s = null;
                    try {
                        Thread.sleep(10000);
                    }
                    catch (Exception e2) {}
                }
            }
        }
    }


    public Client (String host, int port) {
        this.host = host;
        this.port = port;
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
    public void registerNode (Node node) {
        postMessage(new Messages.RegisterNodeMessage(node));
    }

    @Override
    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) {
        postMessage(new Messages.RegisterProbeMessage(probe_id,
                                                      node_id,
                                                      type));
    }

    @Override
    public void startTransaction (int probe_id) {
        postMessage(new Messages.TransactionStartMessage(probe_id));
    }

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) {
        postMessage(new Messages.TransactionEndMessage(probe_id,
                                                       hit_vector));
    }

    @Override
    public void oracle (int probe_id,
                        double error,
                        double confidence) {
        postMessage(new Messages.OracleMessage(probe_id, error, confidence));
    }

    Queue<Message> messages = new LinkedList<Message> ();

    Socket s = null;
    Thread t = null;
    final String client_id = UUID.randomUUID().toString();
    String host;
    int port;
}