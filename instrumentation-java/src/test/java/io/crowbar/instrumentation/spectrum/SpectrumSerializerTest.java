package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.assertEquals;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Transaction;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

public class SpectrumSerializerTest {
    @Test(expected = NullPointerException.class)
    public void testNullSpectrum () {
        SpectrumSerializer.serialize(null);
    }

    @Test
    public void testEmptySerialize () {
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();
        assertEquals("[]\n0 0\n", SpectrumSerializer.serialize(es));
    }

    @Test
    public void testNonEmptySerializeWithoutMetadata () {
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();

        // three components
        es.setProbe(0, ProbeType.TRANSACTION_START, es.getTree().getRoot());
        es.setProbe(1, ProbeType.HIT_PROBE, es.getTree().getRoot());
        es.setProbe(2, ProbeType.TRANSACTION_END, es.getTree().getRoot());

        TransactionFactory<Hit, TrM> tf = new TransactionFactory<Hit, TrM> ();

        // one transaction
        List<Hit> lA = new ArrayList<Hit> ();
        lA.add(new Hit(true));
        lA.add(new Hit(false));
        lA.add(new Hit(true));
        Transaction<Hit, TrM> transaction = tf.create(0, lA, 1.0, 0.0, null);
        es.setTransaction(transaction);

        assertEquals(3, es.getProbeCount());
        assertEquals(1, es.getTransactionCount());

        String output = SpectrumSerializer.serialize(es);
        String except = "[0: (TransactionStart, root), 1: (HitProbe, root), 2: (TransactionEnd, root)]\n" +
                        "3 1\n" +
                        "1 0 1 1.0 # exceptionClass: null, exceptionMessage: null\n";

        assertEquals(except, output);
    }

    @Test
    public void testNonEmptySerializeWithMetadata () {
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();

        // three components
        es.setProbe(0, ProbeType.TRANSACTION_START, es.getTree().getRoot());
        es.setProbe(1, ProbeType.HIT_PROBE, es.getTree().getRoot());
        es.setProbe(2, ProbeType.TRANSACTION_END, es.getTree().getRoot());

        TransactionFactory<Hit, TrM> tf = new TransactionFactory<Hit, TrM> ();

        // one transaction
        List<Hit> lA = new ArrayList<Hit> ();
        lA.add(new Hit(true));
        lA.add(new Hit(false));
        lA.add(new Hit(true));
        Transaction<Hit, TrM> transaction = tf.create(0, lA, 1.0, 0.0, new TrM("exception class", "exception message"));
        es.setTransaction(transaction);

        String output = SpectrumSerializer.serialize(es);
        String except = "[0: (TransactionStart, root), 1: (HitProbe, root), 2: (TransactionEnd, root)]\n" +
                        "3 1\n" +
                        "1 0 1 1.0 # exceptionClass: exception class, exceptionMessage: exception message\n";

        assertEquals(except, output);
    }
}