package io.crowbar.util.io;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer extends ThreadedServer {
    private class EchoService implements Runnable {
        public EchoService (Socket s) {
            this.socket = s;
        }

        public void run () {
            try {
                while (socket.isConnected()) {
                    InputStream in = socket.getInputStream();
                    OutputStream out = socket.getOutputStream();
                    out.write(in.read());
                }

                socket.close();
            } catch (IOException e) {
            	e.printStackTrace();
            }
        }

        private Socket socket;
    }


    public EchoServer (ServerSocket serverSocket) {
        super(serverSocket);
    }

    @Override
    protected Runnable handle (Socket s) {
        return new EchoService(s);
    }
}