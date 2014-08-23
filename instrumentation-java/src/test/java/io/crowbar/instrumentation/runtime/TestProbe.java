package io.crowbar.instrumentation.runtime;

import static org.junit.Assert.assertEquals;
import io.crowbar.diagnostic.spectrum.ProbeType;

import org.junit.Test;

public class TestProbe {

    @Test
    public void testGetNodeID() {
        Probe p = new Probe();
        assertEquals(-1, p.getNodeId());
    }

    @Test
    public void testGetID() {
        Probe p = new Probe();
        assertEquals(-1, p.getId());
    }

    @Test
    public void testConstructor() {
        Probe p0 = new Probe();
        assertEquals("[Probe: (id: -1, nodeId: -1, type: null)]", new Probe(p0).toString());
    }

    @Test
    public void testGetType() {
        Probe p = new Probe(0, 0, ProbeType.TRANSACTION_START);
        assertEquals(ProbeType.TRANSACTION_START, p.getType());
    }
}
