package io.crowbar.diagnostic.spectrum.matchers;

import static org.junit.Assert.assertEquals;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;

import java.util.BitSet;
import java.util.Scanner;

import org.junit.Test;

public class NegateMatcherTest {
    @Test
    public void testTransactions () {
        ValidTransactionMatcher v = new ValidTransactionMatcher();
        NegateMatcher n = new NegateMatcher(v);

        String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";


        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        BitSet bs = n.matchTransactions(s);


        assertEquals(bs.length(), 0);
    }

    @Test
    public void testProbes () {
        ValidTransactionMatcher v = new ValidTransactionMatcher();
        NegateMatcher n = new NegateMatcher(v);

        String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";


        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        BitSet bs = n.matchProbes(s);


        assertEquals(bs.length(), 0);
    }
}