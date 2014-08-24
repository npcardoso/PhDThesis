package io.crowbar.instrumentation.runtime;

import static org.junit.Assert.*;
import junit.framework.Assert;

import org.junit.Test;

public class CollectorTest {

    @Test
    public void testNullCollector() {
        Assert.assertNull(Collector.instance());
    }

    @Test
    public void testNotNullCollector() {
        Collector.start("", null);
        Assert.assertNotNull(Collector.instance());
    }

    @Test
    public void testRegisterNode() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testRegisterProbe() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testTransactionStart() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testTransactionEnd() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testTransactionEnd2() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testOracle() {
        Collector.start("", null);
        Collector c = Collector.instance();
        c.oracle(0, 0.0, 0.0);
        // FIXME: catch NullPointerException
    }

    @Test
    public void testGetHitVectorEmptyCollector() {
        Collector.start("", null);
        Collector c = Collector.instance();

        try {
            c.getHitVector("");
            fail("NullPointerException: key doesn't exist");
        } catch(NullPointerException e) {
            // empty
        }
    }

    @Test
    public void testGetHitVectorNonEmptyCollector() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testHitEmptyCollector() {
        Collector.start("", null);
        Collector c = Collector.instance();

        try {
            c.hit(-1);
            fail("IndexOutOfBoundsException");
        } catch(IndexOutOfBoundsException e) {
            // empty
        }
    }

    @Test
    public void testHitNonEmptyCollector() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testGetRootNode() {
        Collector.start("root", null);
        Collector c = Collector.instance();
        Assert.assertNotNull(c.getRootNode());
        Assert.assertEquals("root", c.getRootNode().getName());
    }
}
