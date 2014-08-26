package io.crowbar.instrumentation;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class OracleMessageTest {
    @Test
    public void testEmptyConstructor () {
        Messages.OracleMessage mo = new Messages.OracleMessage();
        assertEquals(-1.0, mo.getConfidence(), 0.0);
        assertEquals(-1.0, mo.getError(), 0.0);
        assertEquals(-1, mo.getProbeId());
    }

    @Test
    public void testNonEmptyConstructor () {
        Messages.OracleMessage mo = new Messages.OracleMessage(2, 0.0, 0.0);
        assertEquals(0.0, mo.getConfidence(), 0.0);
        assertEquals(0.0, mo.getError(), 0.0);
        assertEquals(2, mo.getProbeId());
    }
}