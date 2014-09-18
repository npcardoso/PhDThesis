package io.crowbar.diagnostic.spectrum.matchers;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.EditableTree;
import io.crowbar.diagnostic.spectrum.HitTransaction;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.matchers.SpectrumMatcher;

import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;

public class TestProbesMatcherTest {
    private static final SpectrumMatcher m = new TestProbesMatcher();

    public EditableSpectrum setUp () {
        BitSet activities = new BitSet();


        activities.clear(0);
        activities.set(1);
        activities.clear(2);
        activities.set(3);
        activities.set(4);
        activities.set(5);
        activities.set(6);


        EditableSpectrum es = new EditableSpectrum();

        Transaction transaction = new HitTransaction(0, activities, 1.0, 0.0);
        es.setTransaction(transaction);


        EditableTree t = es.getTree();
        t.addNode("other node", null, 0);
        t.addNode("other node", null, 0);
        t.addNode("other node", null, 1);
        t.addNode("other node", null, 1);
        t.addNode("other node", null, 2);
        t.addNode("other node", null, 2);

        for (int i = 0; i < 7; i++) {
            es.setProbe(6 - i, ProbeType.HIT_PROBE, es.getTree().getNode(i));
        }

        return es;
    }

    @Test
    public void testEmpty () {
        EditableSpectrum s = setUp();


        assertEquals(true, m.matchProbes(s).isEmpty());
        assertEquals(false, m.matchTransactions(s).isEmpty());
    }

    @Test
    public void testMatchProbesLeaf () {
        for (int i = 3; i < 7; i++) {
            EditableSpectrum s = setUp();

            s.setProbe(6 - i, ProbeType.TRANSACTION_START, s.getTree().getNode(i));
            BitSet b = m.matchProbes(s);

            assertEquals(1, b.cardinality());
            assertEquals(true, b.get(6 - i));
        }

        EditableSpectrum s = setUp();

        for (int i = 3; i < 7; i++) {
            s.setProbe(6 - i, ProbeType.TRANSACTION_START, s.getTree().getNode(i));
            BitSet b = m.matchProbes(s);

            assertEquals(i - 2, b.cardinality());
            assertEquals(true, b.get(6 - i));
        }
    }

    @Test
    public void testMatchProbesRoot () {
        EditableSpectrum s = setUp();


        s.setProbe(0, ProbeType.TRANSACTION_START, s.getTree().getRoot());

        BitSet b = m.matchProbes(s);

        for (int i = 0; i < 7; i++) {
            assertEquals(true, b.get(i));
        }

        assertEquals(7, b.cardinality());
    }

    @Test
    public void testMatchProbesBranch () {
        EditableSpectrum s = setUp();


        s.setProbe(5, ProbeType.TRANSACTION_START, s.getTree().getNode(1));

        BitSet b = m.matchProbes(s);

        assertEquals(3, b.cardinality());
        assertTrue(b.get(5));
        assertTrue(b.get(3));
        assertTrue(b.get(2));
    }
}