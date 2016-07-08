package io.crowbar.util.io;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public abstract class ThreadedServer extends Thread {
    private class Handler implements Runnable {
        private final Socket socket;
        Handler (Socket socket) {
            this.socket = socket;
        }

        public void start () {
            Thread t = new Thread(threads, this);


            t.start();
        }

        @Override
        public void run () {
            Runnable r = handle(socket);


            if (r != null)
                r.run();
        }
    }

    public ThreadedServer (ServerSocket serverSocket) {
        this.serverSocket = serverSocket;
    }

    @Override
    public final void run () {
        while (!serverSocket.isClosed()) {
            try {
                Socket s = serverSocket.accept();
                handleRequest(s);
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private synchronized void handleRequest (Socket s) throws IOException {
        // Limit number of clients
        if (maxClients > 0 && threads.activeCount() >= maxClients) {
            s.close();
            return;
        }

        // Limit number of requests
        if (maxRequests == 0) {
            s.close();
            serverSocket.close();
            return;
        }


        Handler h = new Handler(s);
        h.start();

        if (maxRequests > 0)
            maxRequests--;

        // Limit number of requests
        if (maxRequests == 0) {
            serverSocket.close();
            return;
        }
    }

    public final synchronized void setMaxClients (int maxClients) {
        this.maxClients = maxClients;
    }

    public final synchronized void setMaxRequests (int maxRequests) {
        this.maxRequests = maxRequests;
    }

    protected abstract Runnable handle (Socket s);
    private ThreadGroup threads = new ThreadGroup(""); // TODO: What's the purpose of name?
    private ServerSocket serverSocket;
    private int maxClients = -1;
    private int maxRequests = -1;
}
