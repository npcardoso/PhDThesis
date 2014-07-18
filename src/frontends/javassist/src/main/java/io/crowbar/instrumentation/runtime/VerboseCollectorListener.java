package io.crowbar.instrumentation.runtime;


public class VerboseCollectorListener implements CollectorListener {
    public void startTransaction (Collector c,
                                  Probe p) {
        System.out.println("!!!!!!!!! transaction start @ (" + p.getProbeSet().getName() +
                           ", " + p.getId() + ") !!!!!!!!!");
    }

    public void endTransaction (Collector c,
                                Probe p,
                                boolean[] hit_vector) {
        int i = 0;


        for (ProbeSet ps : c.getProbeSets()) {
            for (int j = 0; j < ps.size(); j++) {
                System.out.print(i++);
                System.out.println(": " + ps.getName() + ps.get(j));
            }
        }

        for (boolean b : hit_vector)
            System.out.print(b ? "1 " : "0 ");

        System.out.println("\n!!!!!!!!! transaction end @ (" + p.getProbeSet().getName() +
                           ", " + p.getId() + ") !!!!!!!!!");
    }

    public void oracle (Collector c,
                        Probe p,
                        double error,
                        double confidence) {
        System.out.println("\n!!!!!!!!! collecting oracle @ (" + p.getProbeSet().getName() +
                           ", " + p.getId() + ") : " + error +
                           ", " + confidence + " !!!!!!!!!");
    }

    public void register (Collector c,
                          ProbeSet ps) {
        System.out.println("\n!!!!!!!!! registering probeset !!!!!!!!!");

        for (int j = 0; j < ps.size(); j++) {
            System.out.println(j + ": " + ps.getName() + ps.get(j));
        }
    }
}