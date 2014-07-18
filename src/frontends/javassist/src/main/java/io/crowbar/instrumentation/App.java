package io.crowbar.instrumentation;

import io.crowbar.instrumentation.runtime.*;
import io.crowbar.util.io.*;

import static org.junit.Assert.*;
import org.junit.Test;
import java.net.ServerSocket;
import java.io.*;
import java.net.Socket;


import javassist.*;

public class App {
    public static class EchoServer2 extends ThreadedServer {
        private class EchoService implements Runnable {
            public EchoService (Socket s) {
                this.socket = s;
            }

            @Test
            public void run () {
                try {
                    while (socket.isConnected()) {
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


        public EchoServer2 (ServerSocket server_socket) {
            super(server_socket);
        }

        @Override
        protected Runnable handle (Socket s) {
            return new EchoService(s);
        }
    }


    public static boolean[] ASDsdaf = new boolean[1235];

    private static class brogle {
        private int trackles (int a, float b) {
            if (a > b)
                return a;
            else
                for (; a < b; a++) {
                    if (a + "" == "123")
                        return a;
                }

            return (int) b;
        }

        private static void broglestar (final brogle asd) {
            ASDsdaf[444] = true;
        }

        @Test
        public void discoversExpiredCreditCard () {
            assertEquals(1, 1);
        }
    }

    public static void main (String[] args) {
        // Collector.getDefault().hitprobe(543);
        brogle b = new brogle();


        b.discoversExpiredCreditCard();
        /*try {
         *  ThreadedServer server = new EchoServer2(new ServerSocket(1234));
         *  server.max_clients = 2;
         *  server.start();
         *  server.join();
         * }
         * catch (Exception e) {
         *  e.printStackTrace();
         * }*/
    }

    private int blurz (int a, float b) {
        if (a > b)
            return a;
        else
            for (; a < b; a++) {
                if (a + "" == "123")
                    return a;
            }

        return (int) b;
    }

    private void tracklerbar () {
        ASDsdaf[444] = true;
    }

    private int crasps () {
        return 9999;
    }
}