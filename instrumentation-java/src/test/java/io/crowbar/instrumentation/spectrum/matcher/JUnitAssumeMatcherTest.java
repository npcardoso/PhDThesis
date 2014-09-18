package io.crowbar.instrumentation.spectrum.matcher;

import static org.junit.Assert.assertEquals;
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


        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();
        assertEquals(0, jm.matchTransactions(es).cardinality());
        assertEquals(0, jm.matchProbes(es).cardinality());
    }

    @Test
    public void testNullMetadata () {
        JUnitAssumeMatcher jm = new JUnitAssumeMatcher(false);


        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();

        TransactionFactory<Hit, TrM> tf = new TransactionFactory<Hit, TrM> ();
        Transaction<Hit, TrM> transaction = tf.create(0, activities, 1.0, 0.0, null);
        es.setTransaction(transaction);

        assertEquals(0, jm.matchProbes(es).cardinality());
        assertEquals(0, jm.matchTransactions(es).cardinality());
    }

    @Test
    public void testTrueDefaultValueProbes () {
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();

        TransactionFactory<Hit, TrM> tf = new TransactionFactory<Hit, TrM> ();
        Transaction<Hit, TrM> transaction = tf.create(0, activities, 1.0, 0.0, new TrM("", ""));
        es.setTransaction(transaction);

        JUnitAssumeMatcher jm = new JUnitAssumeMatcher(true);
        assertEquals(activities.size(), jm.matchProbes(es).cardinality());
        assertEquals(0, jm.matchTransactions(es).cardinality());
    }

    @Test
    public void testFalseDefaultValueProbes () {
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();

        TransactionFactory<Hit, TrM> tf = new TransactionFactory<Hit, TrM> ();
        Transaction<Hit, TrM> transaction = tf.create(0, activities, 1.0, 0.0, new TrM("", ""));
        es.setTransaction(transaction);

        JUnitAssumeMatcher jm = new JUnitAssumeMatcher(false);
        assertEquals(0, jm.matchProbes(es).cardinality());
        assertEquals(0, jm.matchTransactions(es).cardinality());
    }

    @Test
    public void testMatchTransactions () {
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();

        TransactionFactory<Hit, TrM> tf = new TransactionFactory<Hit, TrM> ();
        Transaction<Hit, TrM> transaction = tf.create(0, activities, 1.0, 0.0, new TrM("org.junit.Assume$AssumptionViolatedException", ""));
        es.setTransaction(transaction);

        JUnitAssumeMatcher jm = new JUnitAssumeMatcher();
        assertEquals(1, jm.matchTransactions(es).cardinality());
    }
}