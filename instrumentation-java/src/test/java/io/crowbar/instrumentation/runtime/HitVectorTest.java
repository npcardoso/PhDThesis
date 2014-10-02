package io.crowbar.instrumentation.runtime;

import static org.junit.Assert.*;

import java.util.Arrays;

import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;

import org.junit.Test;

public class HitVectorTest {
    @Test
    public void testRegisterProbeEmptyHitVector () {
        HitVector hv = new HitVector();
        HitProbe hp = hv.registerProbe(null, -1, ProbeType.ORACLE);


        assertEquals(-1, hp.getNodeId());
    }

    @Test
    public void testRegisterProbe () {
        HitVector hv = new HitVector();


        populateHitVector(hv);

        assertEquals(hv.get("group1").length, 3);
        assertEquals(hv.get("group2").length, 1);
        assertEquals(hv.get().length, 4);
    }

    @Test
    public void testHitProbe () {
        HitVector hv = new HitVector();


        populateHitVector(hv);

        assertTrue(Arrays.equals(hv.get(), new boolean[] {true, false, false, true}));
    }

    @Test
    public void testReset () {
        HitVector hv = new HitVector();


        populateHitVector(hv);
        hv.reset();

        assertTrue(Arrays.equals(hv.get(), new boolean[] {false, false, false, false}));
    }

    private void populateHitVector (HitVector hv) {
        hv.registerProbe("group1", 0, ProbeType.HIT_PROBE);
        hv.registerProbe("group1", 0, ProbeType.HIT_PROBE);
        hv.registerProbe("group1", 0, ProbeType.HIT_PROBE);
        hv.registerProbe("group2", 0, ProbeType.HIT_PROBE);

        hv.get("group1");
        hv.get("group2");
        hv.hit(0);
        hv.hit(3);
    }

    @Test
    public void testExistsHitVector () {
        HitVector hv = new HitVector();


        assertEquals(false, hv.exists("key"));
    }

    @Test(expected = NullPointerException.class)
    public void testNullGet () {
        HitVector hv = new HitVector();


        hv.get("group");
    }

    @Test(expected = Exception.class)
    public void testGetArrayWithKeyException () {
        HitVector hv = new HitVector();


        hv.get("key");
    }

    @Test
    public void testGetEmptyHitVector () {
        HitVector hv = new HitVector();


        assertEquals(0, hv.get().length);
    }

    @Test(expected = IndexOutOfBoundsException.class)
    public void testHitIndexOutOfBoundsException () {
        HitVector hv = new HitVector();


        hv.hit(0);
    }
}