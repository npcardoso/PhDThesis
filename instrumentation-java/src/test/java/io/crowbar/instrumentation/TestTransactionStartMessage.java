package io.crowbar.instrumentation;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class TestTransactionStartMessage {

    @Test
    public void testEmptyConstructor() {
        Messages.TransactionStartMessage mt = new Messages.TransactionStartMessage();
        assertEquals(-1, mt.getProbeId());

        assertEquals("[[TransactionStartMessage]: probe_id: -1]", mt.toString());
    }

    @Test
    public void testNonEmptyConstructor() {
        Messages.TransactionStartMessage mt = new Messages.TransactionStartMessage(2);
        assertEquals(2, mt.getProbeId());
  
        assertEquals("[[TransactionStartMessage]: probe_id: 2]", mt.toString());
    }
}
