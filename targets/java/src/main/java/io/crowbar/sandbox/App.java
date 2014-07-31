package io.crowbar.sandbox;

import io.crowbar.util.io.ThreadedServer;

import static org.junit.Assert.assertEquals;
import org.junit.Test;
import org.junit.Assume;

import java.net.ServerSocket;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;

public class App {
    public static final class EchoServer2 extends ThreadedServer {
        private final class EchoService implements Runnable {
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
                catch (IOException e) {
                    e.printStackTrace();
                }
            }

            private Socket socket;
        }


        public EchoServer2 (ServerSocket serverSocket) {
            super(serverSocket);
        }

        @Override
        protected Runnable handle (Socket s) {
            return new EchoService(s);
        }

        @Test
        public void discoversExpiredCreditCard2 () {
            assertEquals(1, 1);
        }
    }


    private static boolean[] aSDsdaf = new boolean[1235];

    private static class Brogle {
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

        private static void broglestar (final Brogle asd) {
            aSDsdaf[444] = true;
        }

        @Test
        public void discoversExpiredCreditCard () {
            assertEquals(1, 1);
        }

        @Test
        public void discoversExpiredCreditCard2 () {
            assertEquals(1, 0);
        }

        @Test
        public void testAssume () {
            Assume.assumeTrue(1 == 0);
            assertEquals(1, 0);
        }

        @Test(expected = IndexOutOfBoundsException.class)
        public void testIndexOutOfBoundsException () {
            ArrayList emptyList = new ArrayList();
            Object o = emptyList.get(0);
        }
    }

    public static void main (String[] args) {
        // Collector.getDefault().hitprobe(543);
        Brogle b = new Brogle();


        try {b.discoversExpiredCreditCard();} catch (Exception e) {}
        try {b.testAssume();} catch (Exception e) {}
        try {b.testIndexOutOfBoundsException();} catch (Throwable e) {}
        try {b.discoversExpiredCreditCard2();} catch (Throwable e) {}


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
        aSDsdaf[444] = true;
    }

    private int crasps () {
        return 9999;
    }
}