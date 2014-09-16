package io.crowbar.instrumentation.spectrum.matcher;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.TransactionFactory;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.instrumentation.spectrum.TrM;

import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;

public class ProbeTypeMatcherTest {
    private static List<Hit> activities = new ArrayList<Hit> ();

    @BeforeClass
    public static void setUp () {
        activities.add(new Hit(false));
        activities.add(new Hit(true));
        activities.add(new Hit(false));
    }

    @Test
    public void testEmpty () {
        ProbeTypeMatcher ptm = new ProbeTypeMatcher(ProbeType.TRANSACTION_START);


        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();
        assertEquals(true, ptm.matchProbes(es).isEmpty());
        assertEquals(true, ptm.matchTransactions(es).isEmpty());
    }

    @Test
    public void testFalseDefaultValueTransactions () {
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();

        TransactionFactory<Hit, TrM> tf = new TransactionFactory<Hit, TrM> ();
        Transaction<Hit, TrM> transaction = tf.create(0, activities, 1.0, 0.0, null);
        es.setTransaction(transaction);

        ProbeTypeMatcher ptm = new ProbeTypeMatcher(true, ProbeType.TRANSACTION_START);
        assertEquals(false, ptm.matchTransactions(es).isEmpty());
    }

    @Test
    public void testTrueDefaultValueTransactions () {
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();

        TransactionFactory<Hit, TrM> tf = new TransactionFactory<Hit, TrM> ();
        Transaction<Hit, TrM> transaction = tf.create(0, activities, 1.0, 0.0, null);
        es.setTransaction(transaction);

        ProbeTypeMatcher ptm = new ProbeTypeMatcher(false, ProbeType.TRANSACTION_START);
        assertEquals(true, ptm.matchTransactions(es).isEmpty());
    }

    @Test
    public void testMatchProbes () {
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();

        Node n = es.getTree().addNode("other node", null, 0);
        es.setProbe(0, ProbeType.HIT_PROBE, n);
        es.setProbe(1, ProbeType.ORACLE, n);
        es.setProbe(2, ProbeType.TRANSACTION_START, n);

        TransactionFactory<Hit, TrM> tf = new TransactionFactory<Hit, TrM> ();
        Transaction<Hit, TrM> transaction = tf.create(0, activities, 1.0, 0.0, null);
        es.setTransaction(transaction);

        ProbeTypeMatcher ptm = new ProbeTypeMatcher(ProbeType.TRANSACTION_START);
        BitSet b = ptm.matchProbes(es);
        assertEquals(1, b.cardinality());
        assertEquals(3, b.length());
        assertFalse(b.get(0));
        assertFalse(b.get(1));
        assertTrue(b.get(2));
    }
}