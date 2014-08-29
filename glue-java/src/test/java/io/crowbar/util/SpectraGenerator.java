package io.crowbar.util;

import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.EditableTree;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.TransactionFactory;
import io.crowbar.diagnostic.spectrum.activity.Hit;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public final class SpectraGenerator {
    // TODO: Improve generator.
    public static Spectrum<Hit, ? > generateSpectrum (int ntrans,
                                                      int nprobes,
                                                      int nodes,
                                                      double actRate,
                                                      double errRate) {
        EditableTree et = new EditableTree("");


        // Node n = et.getRoot();

        for (int p = 0; p < nodes; p++) {
            Node c1 = et.addNode("foo" + p, 0);
        }

        EditableSpectrum s = new EditableSpectrum();

        TransactionFactory<Hit, ? > tf = new TransactionFactory();

        for (int t = 0; t < ntrans; t++) {
            List<Hit> activities = new ArrayList<Hit> (nprobes);

            for (int i = 0; i < nprobes; i++) {
                if (Math.random() <= actRate) {
                    activities.add(new Hit(true));
                } else {
                    activities.add(new Hit(false));
                }

                s.setProbe(i, ProbeType.HIT_PROBE, et.getNode(i));
            }

            s.setProbe(nprobes - 1, ProbeType.ORACLE, et.getNode(nprobes - 1));

            Transaction<Hit, ? > transaction;

            if (Math.random() <= errRate) {
                transaction = tf.create(t, activities, 1.0, 0.0, null);
            } else {
                transaction = tf.create(t, activities, 0.0, 0.0, null);
            }

            s.setTransaction(transaction);
        }

        return s;
    }

    static List<Double> randomizeScores (int numNodes) {
        List<Double> l = new ArrayList<Double> ();
        Random r = new Random();

        while (numNodes-- > 0) {
            l.add(r.nextDouble());
        }

        return l;
    }
}