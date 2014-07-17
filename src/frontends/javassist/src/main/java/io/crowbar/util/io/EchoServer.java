package io.crowbar.util.io;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer extends ThreadedServer {
    private class EchoService implements Runnable {
        public EchoService(Socket s){
            this.socket = s;
        }
        public void run(){
            try {
                while(socket.isConnected()) {
                    InputStream in = socket.getInputStream();
                    OutputStream out = socket.getOutputStream();
                    out.write(in.read());
                }
                socket.close();
            }
            catch (IOException e) {}
        }
        Socket socket;

    }


    public EchoServer(ServerSocket server_socket) {
        super(server_socket);
    }



    @Override
    protected Runnable handle(Socket s) {
        return new EchoService (s);
    }


}
