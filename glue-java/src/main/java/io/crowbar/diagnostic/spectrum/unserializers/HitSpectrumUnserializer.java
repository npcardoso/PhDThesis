package io.crowbar.diagnostic.spectrum.unserializers;


import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.TransactionFactory;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.Spectrum;

import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;


public final class HitSpectrumUnserializer {
    public static Spectrum<Hit, ? > unserialize (Scanner s) {
        EditableSpectrum spectrum = new EditableSpectrum();

        int numComp = s.nextInt();
        int numTran = s.nextInt();


        for (int i = 0; i < numTran; i++) {
            spectrum.setTransaction(HitSpectrumUnserializer.unserialize(s, i, numComp));
        }

        return spectrum;
    }

    public static Transaction<Hit, ? > unserialize (Scanner s,
                                                    int id,
                                                    int componentCount) {
        List<Hit> activity = new ArrayList<Hit> ();

        for (int i = 0; i < componentCount; i++) {
            int a = s.nextInt();
            activity.add(new Hit(a != 0));
        }

        double error = 0;

        if (s.hasNextDouble()) {
            error = s.nextDouble();
        }
        else {
            String errStr = s.next();
            System.out.println(errStr);

            error = (errStr.equals("-") || errStr.equals("x") || errStr.equals("X")) ? 1 : 0;
        }

        return new TransactionFactory().create(id, activity, error, 1, null);
    }
}