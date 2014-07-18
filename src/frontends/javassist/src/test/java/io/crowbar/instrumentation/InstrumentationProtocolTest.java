package io.crowbar.instrumentation;
import org.junit.*;
import static org.junit.Assert.*;

public class InstrumentationProtocolTest {
    public void test_ClassSendRcv (InstrumentationProtocol ip) {
        String name = "ASDasd1dqsdas!!";


        ProbeSet ps (name);

        assertTrue(ip.rcvProbeSet(ip.sendProbeSet(ps).getName() == ps.getName()));

        int id = 123;
        ps.prepare(id);
        assertTrue(ip.rcvProbeSet(ip.sendProbeSet(ps).getId() == id));
    }
}