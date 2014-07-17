package io.crowbar.instrumentation.runtime;

import java.util.*;


public class Collector {
    public static Collector getDefault() {
        return collector;
    }

    public void transactionStart (String class_name, int probe_id) {
        System.out.println("!!!!!!!!! transaction start @ (" + class_name + "," + probe_id + ") !!!!!!!!!");
    }

    public void transactionEnd (String class_name, int probe_id) throws Exception {
        for (ProbeSet ps : probesets.values()) {
            boolean [] hv = ps.getHitVector();
            for(int i = 0; i < hv.length; i++) {
                System.out.print(hv[i]? "1 " : "0 ");
            }
            ps.resetHitVector();
        }
        System.out.println("");
        System.out.println("!!!!!!!!! transaction end @ (" + class_name + "," + probe_id + ") !!!!!!!!!");
    }


    public void oracle (int id,
                        double error,
                        double confidence) {
        System.out.println("!!!!!!!!! collecting oracle (" + error +
                           "," + confidence +
                           ") @ " + id +
                           "!!!!!!!!!");
    }

    public ProbeSet get (String class_name) {
        return probesets.get(class_name);
    }


    public void register (ProbeSet ps) throws Exception {
        ps.prepare();
        probesets.put(ps.getClassName(), ps);
    }

    public void discard (String class_name) {
        probesets.remove(class_name);
    }

    private static Collector collector = new Collector();
    Map<String,ProbeSet> probesets = new HashMap<String,ProbeSet>();
}
