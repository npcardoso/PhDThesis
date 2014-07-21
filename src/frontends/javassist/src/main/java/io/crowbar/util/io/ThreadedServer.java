package io.crowbar.util.io;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public abstract class ThreadedServer extends Thread {
    public ThreadedServer (ServerSocket server_socket) {
        this.server_socket = server_socket;
    }

    @Override
    public void run () {
        while (!server_socket.isClosed()) {
            try {
                final Socket s = server_socket.accept();

                if (max_clients > 0 && threads.activeCount() >= max_clients) {
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

    protected abstract Runnable handle (Socket s);
    private ThreadGroup threads = new ThreadGroup(""); // TODO: What's the purpose of name?
    private ServerSocket server_socket;
    public int max_clients = 0;
}