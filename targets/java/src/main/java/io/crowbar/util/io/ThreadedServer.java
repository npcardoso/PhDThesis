package io.crowbar.util.io;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public abstract class ThreadedServer extends Thread {
    public ThreadedServer (ServerSocket serverSocket) {
        this.serverSocket = serverSocket;
    }

    @Override
    public final void run () {
        while (!serverSocket.isClosed()) {
            try {
                final Socket s = serverSocket.accept();

                if (maxClients > 0 && threads.activeCount() >= maxClients) {
                    s.close();
                }

                Thread t = new Thread(threads,
                                      new Runnable() {
                                          public void run() {
                                              Runnable r = handle(s);

                                              if (r != null)
                                                  r.run();
                                          }
                                      }
                                      );
                t.start();
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public final void setMaxClients (int maxClients) {
        this.maxClients = maxClients;
    }

    protected abstract Runnable handle (Socket s);
    private ThreadGroup threads = new ThreadGroup(""); // TODO: What's the purpose of name?
    private ServerSocket serverSocket;
    private int maxClients = 0;
}