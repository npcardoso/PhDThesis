package io.crowbar.diagnostic.spectrum.unserializers;

import static org.junit.Assert.*;

import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.Spectrum;

import java.util.Scanner;

import org.junit.Test;

public class HitSpectrumUnserializerTest {
    // TODO: test the individual activities of the spectra.
    // TODO: move spectrum to a class static final (i.e., reduce redundancy).
    // TODO: generate random spectrum, serialize + deserialize it and, finally, compare both.
    @Test
    public void testSerializerTransactionCount () {
        String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";


        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));


        assertEquals(s.getTransactionCount(), 9);
    }

    @Test
    public void testSerializerProbeCount () {
        String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";


        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));


        assertEquals(s.getProbeCount(), 10);
    }

    @Test
    public void testGetProbe () {
        String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";


        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        Probe c = s.getProbe(0);


        assertNull(c);
    }
}