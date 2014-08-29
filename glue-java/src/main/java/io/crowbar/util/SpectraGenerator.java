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
    private static void randomizeTree (EditableSpectrum s,
                                       int numNodes) {
        EditableTree t = s.getTree();
        Random r = new Random();


        while (numNodes-- > 0) {
            t.addNode("foo" + r.nextInt(), r.nextInt(t.size()));
        }
    }

    private static void randomizeProbes (EditableSpectrum s,
                                         int numProbes) {
        EditableTree t = s.getTree();
        Random r = new Random();


        while (numProbes-- > 0) {
            s.setProbe(numProbes,
                       new ProbeType[] {ProbeType.HIT_PROBE,
                                        ProbeType.ORACLE,
                                        ProbeType.TRANSACTION_START,
                                        ProbeType.TRANSACTION_END}[r.nextInt(4)],
                       t.getNode(r.nextInt(t.size())));
        }
    }

    // TODO: Improve generator.
    public static Spectrum<Hit, ? > generateSpectrum (int ntrans,
                                                      int numProbes,
                                                      int numNodes,
                                                      double actRate,
                                                      double errRate) {
        EditableSpectrum s = new EditableSpectrum();


        randomizeTree(s, numNodes);
        randomizeProbes(s, numProbes);


        TransactionFactory<Hit, ? > tf = new TransactionFactory();

        for (int t = 0; t < ntrans; t++) {
            List<Hit> activities = new ArrayList<Hit> (numProbes);

            for (int i = 0; i < numProbes; i++) {
                activities.add(new Hit(Math.random() <= actRate));
            }

            double error = (Math.random() <= errRate) ? 1 : 0;
            s.setTransaction(tf.create(t, activities, error, 1.0, null));
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