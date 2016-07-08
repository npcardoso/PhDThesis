package io.crowbar.util.io;

import static org.junit.Assert.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;

import org.junit.Before;
import org.junit.Test;

public class EchoServerTest {
    @Before
    public void setupServer () {
        ServerSocket s = null;


        try {
            s = new ServerSocket(1122);
        }
        catch (IOException e) {
            fail(e.getStackTrace().toString());
        }

        EchoServer es = new EchoServer(s);
        es.setMaxClients(1);
        es.setMaxRequests(1);
        es.start();
    }

    @Test
    public void testClientConnects () {
        Socket testSocket = null;


        try {
            testSocket = new Socket("localhost", 1122);

            OutputStream out = testSocket.getOutputStream();
            out.write("testing".getBytes());
            out.flush();

            InputStream in = testSocket.getInputStream();
            byte[] b = new byte[10];
            in.read(b);

            assertEquals(new String(b).trim(), "testing");
        }
        catch (Exception e) {
            fail(e.getStackTrace().toString());
        }
    }
}