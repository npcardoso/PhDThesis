package io.crowbar.instrumentation.runtime;

import java.util.*;


public class Collector {
    public static Collector getDefault () {
        return collector;
    }

    public void transactionStart (String class_name, int probe_id) throws Exception {
        System.out.println("!!!!!!!!! transaction start @ (" + class_name + "," + probe_id + ") !!!!!!!!!");

        if (reset_on_transaction_start)
            resetHitVectors();

        probeset_map.get(class_name).getHitVector()[probe_id] = true;
        ;
    }

    public void transactionEnd (String class_name, int probe_id) throws Exception {
        probeset_map.get(class_name).getHitVector()[probe_id] = true;


        boolean[] hitprobes = collectHitVectors();
        resetHitVectors();

        int i = 0;

        for (ProbeSet ps : probeset_list) {
            for (int j = 0; j < ps.size(); j++) {
                System.out.print(i++);
                System.out.println(": " + ps.getClassName() + ps.get(j));
            }
        }

        for (boolean b : hitprobes)
            System.out.print(b ? "1 " : "0 ");

        System.out.println("\n!!!!!!!!! transaction end @ (" + class_name + "," + probe_id + ") !!!!!!!!!");
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
        return probeset_map.get(class_name);
    }

    public boolean[] collectHitVectors () throws ProbeSet.NotPreparedException {
        boolean[] ret = new boolean[getNumProbes()];
        int i = 0;

        for (ProbeSet ps : probeset_list) {
            boolean[] hv = ps.getHitVector();

            for (int j = 0; j < hv.length; j++)
                ret[i++] = hv[j];
        }

        return ret;
    }

    public int getNumProbes () {
        return total_probes;
    }

    public void register (ProbeSet ps) throws Exception {
        ps.prepare();
        probeset_map.put(ps.getClassName(), ps);
        probeset_list.add(ps);
        total_probes += ps.size();
    }

    public void resetHitVectors () {
        for (ProbeSet ps : probeset_list) {
            try {
                ps.resetHitVector();
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private static Collector collector = new Collector ();


    public boolean reset_on_transaction_start = true;
    private Map<String, ProbeSet> probeset_map = new HashMap<String, ProbeSet> ();
    private List<ProbeSet> probeset_list = new ArrayList<ProbeSet> (); // This is needed to maintain serialization order
    private int total_probes = 0;
}