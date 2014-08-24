package io.crowbar.instrumentation.runtime;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;

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
        // TODO
        fail("to implement");
    }

    @Test
    public void testReset () {
        // TODO
        fail("to implement");
    }
}