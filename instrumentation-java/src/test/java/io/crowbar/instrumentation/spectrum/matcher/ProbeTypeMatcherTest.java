package io.crowbar.instrumentation.spectrum.matcher;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.instrumentation.spectrum.TrM;

import org.junit.Test;

public class ProbeTypeMatcherTest {
    @Test
    public void testEmpty () {
        ProbeTypeMatcher ptm = new ProbeTypeMatcher(ProbeType.TRANSACTION_START);


        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM> ();
        assertEquals(true, ptm.matchComponents(es).isEmpty());
        assertEquals(true, ptm.matchTransactions(es).isEmpty());
    }

    @Test
    public void testNonEmptySpectrum () {
        // TODO
        // create a Spectrum with components and transactions
        // test matchTransactions() and matchComponents()
        fail("to implement");
    }
}