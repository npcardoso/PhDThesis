package io.crowbar.instrumentation.runtime;

import static org.junit.Assert.*;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;

import java.util.Arrays;

import org.junit.Test;

public class ProbeGroupTest {
    @Test
    public void testRegister () {
        ProbeGroup pg = new ProbeGroup();
        HitProbe hp = pg.register(1, 2, null);


        assertEquals(1, hp.getId());
        assertEquals(0, hp.getLocalId());
        assertEquals(2, hp.getNodeId());
        assertEquals(null, hp.getType());
        assertEquals(false, hp.getActivation());
    }

    @Test
    public void testGetEmptyProbeGroup () {
        ProbeGroup pg = new ProbeGroup();


        assertEquals(0, pg.get().length);
    }

    @Test
    public void testGet () {
        ProbeGroup pg = new ProbeGroup();


        pg.register(0, 0, ProbeType.HIT_PROBE);
        pg.register(0, 0, ProbeType.HIT_PROBE);
        pg.register(0, 0, ProbeType.HIT_PROBE);
        pg.register(0, 0, ProbeType.HIT_PROBE);
        pg.register(0, 0, ProbeType.HIT_PROBE);

        assertEquals(pg.get().length, 5);
    }

    @Test
    public void testEmptyReset () {
        ProbeGroup pg = new ProbeGroup();


        pg.reset();
        assertEquals(0, pg.get().length);
    }

    @Test
    public void testReset () {
        ProbeGroup pg = new ProbeGroup();


        pg.register(0, 0, ProbeType.HIT_PROBE);
        pg.register(0, 0, ProbeType.HIT_PROBE);

        boolean[] hitArray = pg.get();
        hitArray[0] = true;

        pg.reset();

        assertTrue(Arrays.equals(pg.get(), new boolean[] {false, false}));
    }
}