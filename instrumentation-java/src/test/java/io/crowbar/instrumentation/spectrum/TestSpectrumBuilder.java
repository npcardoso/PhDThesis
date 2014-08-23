package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.instrumentation.runtime.Probe;

import org.junit.Test;

public class TestSpectrumBuilder {

    @Test
    public void testEmptySpectrum() {
        SpectrumBuilder sb = new SpectrumBuilder();
        Spectrum<Hit, TrM> spectrum = sb.getSpectrum();
        assertEquals(0, spectrum.getComponentCount());
        assertEquals(0, spectrum.getTransactionCount());
    }

    @Test
    public void testRegisterNode() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testRegisterProbe() {
        SpectrumBuilder sb = new SpectrumBuilder();
        Probe p = new Probe(0, 0, null);

        try {
            sb.registerProbe(p);
            fail("no probe found");
        } catch(Exception e) {
             // empty
        }
    }

    @Test
    public void testEndTransaction() {
        // TODO
        fail("to implement");
    }

    @Test
    public void testOracle() {
        // TODO
        // create a SpectrumBuilder with one transaction and three components
        // create a probe
        // set the oracle
        fail("to implement");
    }

    @Test
    public void testToString() {
        SpectrumBuilder sb = new SpectrumBuilder();
        Spectrum<Hit, TrM> spectrum = sb.getSpectrum();
        assertEquals("{class='Spectrum', components=[], transactions=[]}", spectrum.toString());
    }
}
