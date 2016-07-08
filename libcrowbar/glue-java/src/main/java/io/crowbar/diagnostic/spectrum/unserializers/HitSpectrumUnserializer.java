package io.crowbar.diagnostic.spectrum.unserializers;


import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.HitTransaction;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Spectrum;

import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;
import java.util.Scanner;


public final class HitSpectrumUnserializer {
    public static Spectrum unserialize (Scanner s) {
        EditableSpectrum spectrum = new EditableSpectrum();

        int numComp = s.nextInt();
        int numTran = s.nextInt();


        for (int i = 0; i < numTran; i++) {
            spectrum.setTransaction(HitSpectrumUnserializer.unserialize(s, i, numComp));
        }

        return spectrum;
    }

    public static Transaction unserialize (Scanner s,
                                           int id,
                                           int probeCount) {
        BitSet activity = new BitSet();


        for (int i = 0; i < probeCount; i++) {
            int a = s.nextInt();
            activity.set(i, a != 0);
        }

        double error = 0;

        if (s.hasNextDouble()) {
            error = s.nextDouble();
        } else {
            String errStr = s.next();

            error = (errStr.equals("-") || errStr.equals("x") || errStr.equals("X")) ? 1 : 0;
        }

        return new HitTransaction(id, activity, error, 1);
    }
}