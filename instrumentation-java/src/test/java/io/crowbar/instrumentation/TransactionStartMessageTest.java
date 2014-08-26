package io.crowbar.instrumentation;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class TransactionStartMessageTest {
    @Test
    public void testEmptyConstructor () {
        Messages.TransactionStartMessage mt = new Messages.TransactionStartMessage();
        assertEquals(-1, mt.getProbeId());
    }

    @Test
    public void testNonEmptyConstructor () {
        Messages.TransactionStartMessage mt = new Messages.TransactionStartMessage(2);
        assertEquals(2, mt.getProbeId());
    }
}