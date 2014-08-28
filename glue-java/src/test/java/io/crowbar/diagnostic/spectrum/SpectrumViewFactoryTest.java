package io.crowbar.diagnostic.spectrum;

import static org.junit.Assert.*;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.matchers.NegateMatcher;
import io.crowbar.diagnostic.spectrum.matchers.ValidTransactionMatcher;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;

import java.util.Scanner;

import org.junit.Test;

public class SpectrumViewFactoryTest {
    @Test
    public void testProbeCount () {
        String in = "3 3 1 0 1 1.0 1 1 0 1.0 1 1 1 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        SpectrumViewFactory<Hit, ? > svf = new SpectrumViewFactory(s);

        SpectrumView<Hit, ? > sv = svf.getView();

        assertEquals(sv.getProbeCount(), 3);
    }

    @Test
    public void testTransactionCount () {
        String in = "3 3 1 0 1 1.0 1 1 0 1.0 1 1 1 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        SpectrumViewFactory<Hit, ? > svf = new SpectrumViewFactory(s);

        SpectrumView<Hit, ? > sv = svf.getView();

        assertEquals(sv.getTransactionCount(), 3);
    }

    @Test
    public void testStage () {
        String in = "3 4 1 0 1 1.0 1 1 0 1.0 1 1 1 0.0 0 0 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        SpectrumViewFactory<Hit, ? > svf = new SpectrumViewFactory(s);

        ValidTransactionMatcher v = new ValidTransactionMatcher();

        svf.addStage(v);

        SpectrumView<Hit, ? > sv = svf.getView();

        assertEquals(sv.getTransactionCount(), 3);
    }
}