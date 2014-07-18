package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.messaging.Messages.Message;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.CollectorListener;
import io.crowbar.instrumentation.runtime.Probe;

import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Queue;
import java.util.LinkedList;

public class AgentClient implements CollectorListener {
    class Dispatcher extends Thread {
        public void run () {
            Message message = getMessage();


            while (message != null) {
                try {
                    if (s == null)
                        s = new Socket(host, port);

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


    public AgentClient (String host, int port) {
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
    public void startTransaction (Collector c,
                                  Probe p) {
        postMessage(new Messages.TransactionStartMessage(p));
    }

    @Override
    public void endTransaction (Collector c,
                                Probe p,
                                boolean[] hit_vector) {
        postMessage(new Messages.TransactionEndMessage(p));
    }

    @Override
    public void oracle (Collector c,
                        Probe p,
                        double error,
                        double confidence) {
        postMessage(new Messages.OracleMessage(p, error, confidence));
    }

    Queue<Message> messages = new LinkedList<Message> ();

    Socket s = null;
    Thread t = null;
    String host;
    int port;
}