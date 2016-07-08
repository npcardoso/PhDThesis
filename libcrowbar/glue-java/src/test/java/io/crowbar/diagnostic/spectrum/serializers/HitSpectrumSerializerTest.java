package io.crowbar.diagnostic.spectrum.serializers;

import static org.junit.Assert.assertTrue;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;

import java.util.Scanner;

import org.junit.Test;
import org.junit.experimental.theories.DataPoint;
import org.junit.experimental.theories.Theories;
import org.junit.experimental.theories.Theory;
import org.junit.runner.RunWith;

@RunWith(Theories.class)
public class HitSpectrumSerializerTest {
    @DataPoint
    public static String in1 = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";
    @DataPoint
    public static String in2 = "3 3 1 0 1 1.0 1 1 0 1.0 1 1 1 0.0";

    @Theory
    public void testSerializer (String in) {
        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));


        assertTrue(HitSpectrumSerializer.serialize(s).toString().replaceAll("\n", " ").trim().equals(in));
    }

    @Theory
    public void testSerializerDelim (String in) {
        String del = "-";


        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));


        assertTrue(HitSpectrumSerializer.serialize(s, del).toString().replaceAll(del, " ").trim().equals(in));
    }

    @Theory
    public void testSerializerTransaction (String in) {
        String del = "-";


        Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));


        for (Transaction t : s.byTransaction()) {
            HitSpectrumSerializer.serialize(t, s.getProbeCount());
        }

        assertTrue(HitSpectrumSerializer.serialize(s, del).toString().replaceAll(del, " ").trim().equals(in));
    }

    @DataPoint
    public static int cTr1 = 10;

    @DataPoint
    public static int cTr2 = 0;


    @Theory
    public void testSerializerNullTransaction (int len) {
        String out = new String(new char[len]).replace("\0", "0 ");


        out = out + "0.0";

        String tr = HitSpectrumSerializer.serialize(null, len);

        assertTrue(tr.equals(out));
    }
}