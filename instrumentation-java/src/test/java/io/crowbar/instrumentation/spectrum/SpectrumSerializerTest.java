package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;

import org.junit.Test;

public class SpectrumSerializerTest {
    @Test
    public void testInstance () {
        SpectrumSerializer ss = new SpectrumSerializer();


        assertNotNull(ss);
    }

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
    public void testNonEmptySerialize () {
        // TODO
        // fail("to implement");
    }
}