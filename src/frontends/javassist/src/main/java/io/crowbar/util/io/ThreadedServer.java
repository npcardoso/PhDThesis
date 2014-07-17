package io.crowbar.util.io;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public abstract class ThreadedServer extends Thread {


    public ThreadedServer(ServerSocket server_socket) {
        this.server_socket = server_socket;
    }

    @Override
    public void run() {
        while(!server_socket.isClosed()) {
            try {
                Socket s = server_socket.accept();

                if (max_clients > 0 && threads.activeCount() >= max_clients) {
                    s.close();
                }

                Runnable r = handle(s);
                Thread t = new Thread(threads, r);
                t.start();

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    protected abstract Runnable handle(Socket s);
    private ThreadGroup threads = new ThreadGroup(""); // TODO: What's the purpose of name?
    private ServerSocket server_socket;
    public int max_clients = 0;
}
