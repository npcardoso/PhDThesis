package io.crowbar.instrumentation.runtime;

import java.util.*;


public class Collector {
    public static Collector getDefault() {
        return collector;
    }

// TODO: Probe with state

    public void transactionStart (int id) {
        System.out.println("!!!!!!!!! starting transaction @ " + id + "!!!!!!!!!");
    }

    public void transactionEnd (int id) {
        for (boolean[] hv : hitvectors) {
            for(int i = 0; i < hv.length; i++) {
                System.out.print(hv[i]?"1 ":"0 ");
                hv[i] = false;
            }
        }
        System.out.println("");
        System.out.println("!!!!!!!!! transaction end @ " + id + "!!!!!!!!!");
    }


    public void oracle (int id,
                        double error,
                        double confidence) {
        System.out.println("!!!!!!!!! collecting oracle (" + error +
                           "," + confidence +
                           ") @ " + id +
                           "!!!!!!!!!");
    }

    public int register (ProbeSet ps) {
        classes.add(ps);
        hitvectors.add(new boolean[ps.size()]);
        return classes.size() - 1;
    }

    public boolean[] getHitVector(int id) {
        return hitvectors.get(id);
    }


    private static Collector collector = new Collector();
    Vector<ProbeSet> classes = new Vector<ProbeSet>();
    Vector<boolean[]> hitvectors = new Vector<boolean[]>();
}
