package io.crowbar.instrumentation;

import static org.junit.Assert.assertEquals;

import org.junit.Assert;
import org.junit.Test;

public class TransactionEndMessageTest {
    @Test
    public void testEmptyConstructor () {
        Messages.TransactionEndMessage mt = new Messages.TransactionEndMessage();
        Assert.assertNull(mt.getExceptionClass());
        Assert.assertNull(mt.getExceptionMessage());
        assertEquals(-1, mt.getProbeId());
        Assert.assertNull(mt.getHitVector());
    }

    @Test
    public void testNonEmptyConstructor () {
        Messages.TransactionEndMessage mt = new Messages.TransactionEndMessage(2, "exceptionClass string", "exceptionMessage string", null);
        assertEquals("exceptionClass string", mt.getExceptionClass());
        assertEquals("exceptionMessage string", mt.getExceptionMessage());
        assertEquals(2, mt.getProbeId());
        Assert.assertNull(mt.getHitVector());
    }
}