package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.*;


public class VerboseListener implements EventListener {
    @Override
    public void startTransaction (Probe p) {
        System.out.println("!!!!!!!!! transaction start @ (" + p.getProbeSet().getName() +
                           ", " + p.getId() + ":" + p + ") !!!!!!!!!");
    }

    @Override
    public void endTransaction (Probe p,
                                boolean[] hit_vector) {
        System.out.println("\n!!!!!!!!! transaction end @ (" + p.getProbeSet().getName() +
                           ", " + p.getId() + ":" + p + ") !!!!!!!!!");

        for (boolean b : hit_vector)
            System.out.print(b ? "1 " : "0 ");

        System.out.println("");
    }

    @Override
    public void oracle (Probe p,
                        double error,
                        double confidence) {
        System.out.println("\n!!!!!!!!! collecting oracle @ (" + p.getProbeSet().getName() +
                           ", " + p.getId() + ":" + p + ") : " + error +
                           ", " + confidence + " !!!!!!!!!");
    }

    @Override
    public void register (ProbeSet ps) {
        System.out.println("\n!!!!!!!!! registering probeset: " + ps.getName() + " !!!!!!!!!");

        for (int j = 0; j < ps.size(); j++) {
            System.out.println(j + ": " + ps.get(j));
        }
    }
}