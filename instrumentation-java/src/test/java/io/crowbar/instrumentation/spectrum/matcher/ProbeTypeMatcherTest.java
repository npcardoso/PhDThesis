package io.crowbar.instrumentation.spectrum.matcher;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.HitTransaction;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.instrumentation.spectrum.TrM;

import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;

public class ProbeTypeMatcherTest {
    private static BitSet activities = new BitSet();

    @BeforeClass
    public static void setUp () {
        activities.clear(0);
        activities.set(1);
        activities.clear(2);
    }

    @Test
    public void testEmpty () {
        ProbeTypeMatcher ptm = new ProbeTypeMatcher(ProbeType.TRANSACTION_START);


        EditableSpectrum es = new EditableSpectrum();


        assertEquals(true, ptm.matchProbes(es).isEmpty());
        assertEquals(true, ptm.matchTransactions(es).isEmpty());
    }

    @Test
    public void testFalseDefaultValueTransactions () {
        EditableSpectrum es = new EditableSpectrum();

        Transaction transaction = new HitTransaction(0, activities, 1.0, 0.0);


        es.setTransaction(transaction);

        ProbeTypeMatcher ptm = new ProbeTypeMatcher(true, ProbeType.TRANSACTION_START);
        assertEquals(false, ptm.matchTransactions(es).isEmpty());
    }

    @Test
    public void testTrueDefaultValueTransactions () {
        EditableSpectrum es = new EditableSpectrum();

        Transaction transaction = new HitTransaction(0, activities, 1.0, 0.0);


        es.setTransaction(transaction);

        ProbeTypeMatcher ptm = new ProbeTypeMatcher(false, ProbeType.TRANSACTION_START);
        assertEquals(true, ptm.matchTransactions(es).isEmpty());
    }

    @Test
    public void testMatchProbes () {
        EditableSpectrum es = new EditableSpectrum();

        Node n = es.getTree().addNode("other node", null, 0);


        es.setProbe(0, ProbeType.HIT_PROBE, n);
        es.setProbe(1, ProbeType.ORACLE, n);
        es.setProbe(2, ProbeType.TRANSACTION_START, n);

        Transaction transaction = new HitTransaction(0, activities, 1.0, 0.0);
        es.setTransaction(transaction);


        System.out.println(es.getProbeCount());
        System.out.println(es);

        ProbeTypeMatcher ptm = new ProbeTypeMatcher(ProbeType.TRANSACTION_START);
        BitSet b = ptm.matchProbes(es);

        assertEquals(1, b.cardinality());
        assertEquals(3, b.length());
        assertFalse(b.get(0));
        assertFalse(b.get(1));
        assertTrue(b.get(2));
    }
}