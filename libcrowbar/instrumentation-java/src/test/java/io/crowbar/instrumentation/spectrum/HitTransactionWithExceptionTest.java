package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.*;

import org.junit.Test;

public class HitTransactionWithExceptionTest {
    @Test
    public void testEmptyExceptionEmptyMessage () {
        HitTransactionWithException tr =
            new HitTransactionWithException(0, 0, 0, null, "", "");


        assertEquals("", tr.getExceptionClass());
        assertEquals("", tr.getExceptionMessage());
    }

    @Test
    public void testGetExceptionClass () {
        String s = "exceptionClass";
        HitTransactionWithException tr =
            new HitTransactionWithException(0, 0, 0, null, s, "");


        assertEquals(s, tr.getExceptionClass());
    }

    @Test
    public void testGetExceptionMessage () {
        String s = "exceptionMessage";
        HitTransactionWithException tr =
            new HitTransactionWithException(0, 0, 0, null, "", s);


        assertEquals(s, tr.getExceptionMessage());
    }
}