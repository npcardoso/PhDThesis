package io.crowbar.instrumentation.spectrum.matcher;

import static org.junit.Assert.*;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.instrumentation.spectrum.TrM;

import org.junit.Test;

public class JUnitAssumeMatcherTest {

    @Test
    public void testEmptySpectrum() {
        JUnitAssumeMatcher jm = new JUnitAssumeMatcher();
        EditableSpectrum<Hit, TrM> es = new EditableSpectrum<Hit, TrM>();
        assertEquals(0, jm.matchTransactions(es).cardinality());
        assertEquals(0, jm.matchComponents(es).cardinality());
    }

    @Test
    public void testNonEmptySpectrum() {
        // TODO
        // create a Spectrum with components and transactions
        // test matchTransactions() and matchComponents()
        fail("to implement");
    }
}
