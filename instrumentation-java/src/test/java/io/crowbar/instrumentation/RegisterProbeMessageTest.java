package io.crowbar.instrumentation;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import io.crowbar.diagnostic.spectrum.ProbeType;

import org.junit.Test;

public class RegisterProbeMessageTest {
    @Test
    public void testEmptyConstructor () {
        // FIXME
        // try {
        //     new Messages.RegisterProbeMessage();
        //     fail("NullPointerException: null Probe");
        // }
        // catch (NullPointerException e) {
        //     // empty
        // }
    }

    @Test
    public void testNonEmptyConstructor () {
        Messages.RegisterProbeMessage mt = new Messages.RegisterProbeMessage(0, 1, ProbeType.HIT_PROBE);
        assertEquals(0, mt.getProbeId());
        assertEquals(1, mt.getNodeId());

        //assertEquals("[[RegisterProbeMessage]: [Probe: (id: 0, nodeId: 1, type: HIT_PROBE)]]", mt.toString());
    }
}
