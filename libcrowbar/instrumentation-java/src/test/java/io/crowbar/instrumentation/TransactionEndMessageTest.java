package io.crowbar.instrumentation;

import static org.junit.Assert.assertEquals;

import org.junit.Assert;
import org.junit.Test;

public class TransactionEndMessageTest {
    @Test
    public void testEmptyConstructor () {
        Messages.TransactionEndMessage mt = new Messages.TransactionEndMessage();
        assertEquals(-1, mt.getProbeId());
        Assert.assertNull(mt.getHitVector());
    }

    @Test
    public void testNonEmptyConstructor () {
        Messages.TransactionEndMessage mt = new Messages.TransactionEndMessage(2, null);
        assertEquals(2, mt.getProbeId());
        Assert.assertNull(mt.getHitVector());
    }
}