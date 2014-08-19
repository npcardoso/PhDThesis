package io.crowbar.diagnosis.spectra.unserializers;


import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.EditableSpectra;
import io.crowbar.diagnosis.spectra.Transaction;
import io.crowbar.diagnosis.spectra.TransactionFactory;
import io.crowbar.diagnosis.spectra.activity.Hit;
import io.crowbar.diagnosis.spectra.Spectra;

import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;


public final class HitSpectraUnserializer {
    public static Spectra<Hit, ?, ?> unserialize (Scanner s) {
        EditableSpectra spectra = new EditableSpectra();

        int numComp = s.nextInt();
        int numTran = s.nextInt();
        for (int i = 0; i < numTran; i++) {
            spectra.setTransaction(HitSpectraUnserializer.unserialize(s, i, numComp));
        }
        return spectra;
    }

    public static Transaction<Hit, ?> unserialize (Scanner s,
                                                      int id,
                                                      int numComponents) {
        List<Hit> activity = new ArrayList<Hit> ();
        for (int i = 0; i < numComponents; i++) {
            int a = s.nextInt();
            activity.add(new Hit(a != 0));
        }

        double error = 0;
        if(s.hasNextDouble()) {
            error = s.nextDouble();
        }
        else {
            String errStr = s.next();
            System.out.println(errStr);

            error = (errStr.equals("-") || errStr.equals("x") || errStr.equals("X"))?1:0;
        }
        return new TransactionFactory().create(id, activity, error, 1, null);
    }
}
