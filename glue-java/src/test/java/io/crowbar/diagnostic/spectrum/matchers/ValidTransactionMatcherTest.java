package io.crowbar.diagnostic.spectrum.matchers;

import static org.junit.Assert.assertEquals;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;

import java.util.BitSet;
import java.util.Scanner;

import org.junit.Test;

public class ValidTransactionMatcherTest {
    @Test
    public void testValidTransactionMatcher () {
        String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";

        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        ValidTransactionMatcher v = new ValidTransactionMatcher();

        BitSet bs = v.matchTransactions(s);


        assertEquals(bs.length(), s.getTransactionCount());
    }
}