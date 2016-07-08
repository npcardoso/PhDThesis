package io.crowbar.instrumentation;

import static org.junit.Assert.assertEquals;
import io.crowbar.diagnostic.spectrum.ProbeType;

import org.junit.Test;

public class RegisterProbeMessageTest {
    @Test
    public void testNonEmptyConstructor () {
        Messages.RegisterProbeMessage mt = new Messages.RegisterProbeMessage(0, 1, ProbeType.HIT_PROBE);
        assertEquals(0, mt.getProbeId());
        assertEquals(1, mt.getNodeId());
    }
}