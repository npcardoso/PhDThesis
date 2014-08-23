package io.crowbar.instrumentation;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.runtime.Probe;

import org.junit.Test;

public class TestRegisterProbeMessage {

    @Test
    public void testEmptyConstructor() {
        try {
            new Messages.RegisterProbeMessage();
            fail("NullPointerException: null Probe");
        } catch (NullPointerException e) {
            // empty
        }
    }

    @Test
    public void testNonEmptyConstructor() {
        Messages.RegisterProbeMessage mt = new Messages.RegisterProbeMessage(new Probe(0, 1, ProbeType.HIT_PROBE));
        assertEquals(0, mt.getProbe().getId());
        assertEquals(1, mt.getProbe().getNodeId());
  
        assertEquals("[[RegisterProbeMessage]: [Probe: (id: 0, nodeId: 1, type: HIT_PROBE)]]", mt.toString());
    }
}
