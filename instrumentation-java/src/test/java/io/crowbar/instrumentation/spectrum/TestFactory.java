package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.*;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.activity.Hit;

import org.junit.Test;

public class TestFactory {

    @Test
    public void testInstance() {
        Transaction<Hit, TrM> transaction = Factory.createTransaction(0, new boolean[3], 1.0, 0.0, null);
        assertNotNull(transaction);
    }

    @Test
    public void testNullHitVector() {
        try {
            Factory.createTransaction(0, null, 0.0, 0.0, null);
            fail("NullPointerException: hitVector == null");
        } catch (NullPointerException e) {
            // empty
        }
    }

    @Test
    public void testGetError() {
        Transaction<Hit, TrM> transaction = Factory.createTransaction(0, new boolean[3], 1.0, 0.0, null);
        assertEquals(1.0, transaction.getError(), 0.0);
    }

    @Test
    public void testIsError() {
        Transaction<Hit, TrM> transaction = Factory.createTransaction(0, new boolean[3], 1.0, 0.0, null);
        assertEquals(true, transaction.isError());
    }

    @Test
    public void testGetConfidence() {
        Transaction<Hit, TrM> transaction = Factory.createTransaction(0, new boolean[3], 0.0, -1.0, null);
        assertEquals(-1.0, transaction.getConfidence(), 0.0);
    }

    @Test
    public void testGetID() {
        Transaction<Hit, TrM> transaction = Factory.createTransaction(5, new boolean[3], 0.0, 0.0, null);
        assertEquals(5, transaction.getId());
    }
}
