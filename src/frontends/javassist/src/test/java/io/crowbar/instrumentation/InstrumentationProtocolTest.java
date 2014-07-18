package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.*;
import io.crowbar.instrumentation.messaging.*;

import java.io.*;

import org.junit.*;
import static org.junit.Assert.*;


public class InstrumentationProtocolTest {
    public void test_ClassSendRcv (InstrumentationProtocol ip) throws Exception {
        String name = "ASDasd1dqsdas!!";


        PipedInputStream in = new PipedInputStream();
        PipedOutputStream out = new PipedOutputStream(in);

        ProbeSet ps = new ProbeSet(name);


        ip.sendProbeSet(out, ps);
        assertTrue(ip.rcvProbeSet(in).getName() == ps.getName());

        int id = 123;
        ps.prepare(id);
        ip.sendProbeSet(out, ps);
        assertTrue(ip.rcvProbeSet(in).getId() == id);
    }
}