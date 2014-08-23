package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.*;

import org.junit.Test;

public class TestTrM {

    @Test
    public void testEmptyExceptionEmptyMessage() {
        TrM tr = new TrM("", "");
        assertEquals("", tr.getExceptionClass());
        assertEquals("", tr.getExceptionMessage());
    }

    @Test
    public void testGetExceptionClass() {
        TrM tr = new TrM("exceptionClass", "");
        assertEquals("exceptionClass", tr.getExceptionClass());
    }

    @Test
    public void testGetExceptionMessage() {
        TrM tr = new TrM("", "exceptionMessage");
        assertEquals("exceptionMessage", tr.getExceptionMessage());
    }
}
