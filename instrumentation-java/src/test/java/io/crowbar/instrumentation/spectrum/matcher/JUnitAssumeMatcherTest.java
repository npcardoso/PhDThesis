package io.crowbar.instrumentation.spectrum.matcher;

import static org.junit.Assert.assertEquals;
import io.crowbar.diagnostic.spectrum.HitTransaction;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.instrumentation.spectrum.TrM;

import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;

public class JUnitAssumeMatcherTest {
    private static BitSet activities = new BitSet();

    @BeforeClass
    public static void setUp () {
        activities.set(0);
        activities.clear(1);
        activities.set(2);
    }

    @Test
    public void testEmptySpectrum () {
        JUnitAssumeMatcher jm = new JUnitAssumeMatcher();


        EditableSpectrum es = new EditableSpectrum();


        assertEquals(0, jm.matchTransactions(es).cardinality());
        assertEquals(0, jm.matchProbes(es).cardinality());
    }

    @Test
    public void testTrueDefaultValueProbes () {
        EditableSpectrum es = new EditableSpectrum();

        Transaction transaction = new HitTransaction(0, activities, 1.0, 0.0);


        es.setTransaction(transaction);

        JUnitAssumeMatcher jm = new JUnitAssumeMatcher(true);
        assertEquals(activities.length(), jm.matchProbes(es).cardinality());
        assertEquals(0, jm.matchTransactions(es).cardinality());
    }

    @Test
    public void testFalseDefaultValueProbes () {
        EditableSpectrum es = new EditableSpectrum();

        Transaction transaction = new HitTransaction(0, activities, 1.0, 0.0);


        es.setTransaction(transaction);

        JUnitAssumeMatcher jm = new JUnitAssumeMatcher(false);
        assertEquals(0, jm.matchProbes(es).cardinality());
        assertEquals(0, jm.matchTransactions(es).cardinality());
    }

    @Test
    public void testMatchTransactions () {
        EditableSpectrum es = new EditableSpectrum();
        /*
         * //TODO: FIX this
         *      Transaction transaction = new HitTransaction(0, activities, 1.0, 0.0, new TrM("org.junit.Assume$AssumptionViolatedException", ""));
         *      es.setTransaction(transaction);
         *
         *      JUnitAssumeMatcher jm = new JUnitAssumeMatcher();
         *      assertEquals(1, jm.matchTransactions(es).cardinality());
         */
    }
}