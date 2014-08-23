package io.crowbar.instrumentation.runtime;

import static org.junit.Assert.*;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;

import org.junit.Test;

public class TestHitVector {

    @Test
    public void testRegisterProbeEmptyHitVector() {
        HitVector hv = new HitVector();
        HitProbe hp = hv.registerProbe(null, -1, ProbeType.ORACLE);
        assertEquals(-1, hp.getNodeId());
    }

    @Test
    public void testRegisterProbe() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testExists() {
        HitVector hv = new HitVector();
        assertEquals(false, hv.exists("key"));
    }

    @Test
    public void testGetArrayWithKeyException() {
        HitVector hv = new HitVector();

        try {
            hv.get("key");
            fail("NullPointerException: key doesn't exist");
        } catch(Exception e) {
            // empty
        }
    }

    @Test
    public void testGetArrayWithKey() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testGetEmptyHitVector() {
        HitVector hv = new HitVector();
        assertEquals(0, hv.get().length);
    }

    @Test
    public void testGet() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testHitIndexOutOfBoundsException() {
        HitVector hv = new HitVector();

        try {
            hv.hit(0);
            fail("IndexOutOfBoundsException");
        } catch(IndexOutOfBoundsException e) {
            // empty
        }
    }

    @Test
    public void testHit() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testReset() {
        // TODO
        fail("to implement");
    }
}
