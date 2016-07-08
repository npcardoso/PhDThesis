package io.crowbar.instrumentation.runtime;

import static org.junit.Assert.assertEquals;
import io.crowbar.diagnostic.spectrum.ProbeType;

import org.junit.Test;

public class ProbeTest {
    @Test
    public void testGetNodeID () {
        Probe p = new Probe();


        assertEquals(-1, p.getNodeId());
    }

    @Test
    public void testGetID () {
        Probe p = new Probe();


        assertEquals(-1, p.getId());
    }

    @Test(expected = NullPointerException.class)
    public void testNullConstructor () {
        new Probe(null);
    }

    @Test
    public void testGetType () {
        Probe p = new Probe(0, 0, ProbeType.TRANSACTION_START);


        assertEquals(ProbeType.TRANSACTION_START, p.getType());
    }
}