package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.instrumentation.runtime.Probe;

import org.junit.Test;

public class SpectrumBuilderTest {
    @Test
    public void testEmptySpectrum () {
        SpectrumBuilder sb = new SpectrumBuilder();


        Spectrum<Hit, TrM> spectrum = sb.getSpectrum();
        assertEquals(0, spectrum.getComponentCount());
        assertEquals(0, spectrum.getTransactionCount());
    }

    @Test
    public void testRegisterNode () throws Exception {
        SpectrumBuilder sb = new SpectrumBuilder();

        sb.registerNode("other node", 1, 0);
        sb.registerProbe(new Probe(0, 1, ProbeType.TRANSACTION_START));
        sb.registerProbe(new Probe(1, 1, ProbeType.TRANSACTION_END));

        System.out.println(sb.getSpectrum().toString());
        fail("to implement");
    }

    @Test(expected = NullPointerException.class)
    public void testRegisterProbe () throws Exception {
        SpectrumBuilder sb = new SpectrumBuilder();
        Probe p = new Probe(0, 0, null);


        sb.registerProbe(p);
    }

    @Test
    public void testEndTransaction () {
        // TODO
        fail("to implement");
    }

    @Test
    public void testOracle () {
        SpectrumBuilder sb = new SpectrumBuilder();
        sb.oracle(0, 0.0, 0.0);
    }

    @Test
    public void testToString () {
        SpectrumBuilder sb = new SpectrumBuilder();


        Spectrum<Hit, TrM> spectrum = sb.getSpectrum();
        assertEquals("{class='Spectrum', components=[], transactions=[]}", spectrum.toString());
    }
}