package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;

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


        sb.registerNode(1, 0, "other node");
        sb.registerProbe(0, 1, ProbeType.TRANSACTION_START);
        sb.registerProbe(1, 1, ProbeType.TRANSACTION_END);

        System.out.println(sb.getSpectrum().toString());
        // fail("to implement");
        // TODO
    }

    // @Test(expected = Exception.class)
    public void testRegisterProbe () throws Exception {
        // SpectrumBuilder sb = new SpectrumBuilder();
        // sb.registerProbe(0,0,null);
    }

    @Test
    public void testEndTransaction () {
        // TODO
        // fail("to implement");
    }

    @Test
    public void testOracle () {
        SpectrumBuilder sb = new SpectrumBuilder();


        sb.oracle(0, 0.0, 0.0);
        // TODO
    }

    @Test
    public void testToString () {
        SpectrumBuilder sb = new SpectrumBuilder();


        Spectrum<Hit, TrM> spectrum = sb.getSpectrum();
        assertEquals("{class='Spectrum', components=[], transactions=[]}", spectrum.toString());
    }
}