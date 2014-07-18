package io.crowbar.instrumentation.runtime;

import java.util.*;


public class Collector {
    public static Collector getDefault () {
        return collector;
    }

    public void addListener (CollectorListener cl) {
        assert cl != null;
        this.listeners.add(cl);
    }

    public void transactionStart (String class_name,
                                  int probe_id) throws Exception {
        ProbeSet ps = probeset_map.get(class_name);


        assert ps != null;

        Probe p = ps.get(probe_id);
        assert p != null;

        if (reset_on_transaction_start)
            resetHitVectors();

        ps.getHitVector()[probe_id] = true;

        for (CollectorListener cl : listeners) {
            cl.startTransaction(this, p);
        }
    }

    public void transactionEnd (String class_name,
                                int probe_id) throws ProbeSet.NotPreparedException {
        ProbeSet ps = probeset_map.get(class_name);


        assert ps != null;

        Probe p = ps.get(probe_id);
        assert p != null;

        ps.getHitVector()[probe_id] = true;

        boolean[] hit_vector = collectHitVectors();

        for (CollectorListener cl : listeners) {
            cl.endTransaction(this, p, hit_vector);
        }

        resetHitVectors();
    }

    public void oracle (String class_name,
                        int probe_id,
                        double error,
                        double confidence) throws ProbeSet.NotPreparedException {
        ProbeSet ps = probeset_map.get(class_name);


        assert ps != null;

        Probe p = ps.get(probe_id);
        assert p != null;

        ps.getHitVector()[probe_id] = true;

        for (CollectorListener cl : listeners) {
            cl.oracle(this, p, error, confidence);
        }
    }

    // TODO: move the rest of this class to a different class
    public List<ProbeSet> getProbeSets () {
        return probeset_list;
    }

    public ProbeSet get (String name) {
        return probeset_map.get(name);
    }

    public ProbeSet get (int probeset_id) {
        return probeset_list.get(probeset_id);
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
        ps.prepare(probeset_list.size());
        probeset_map.put(ps.getName(), ps);
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


    private Map<String, ProbeSet> probeset_map = new HashMap<String, ProbeSet> ();
    private List<ProbeSet> probeset_list = new ArrayList<ProbeSet> (); // This is needed to maintain serialization order
    private int total_probes = 0;

    public boolean reset_on_transaction_start = true;
    private List<CollectorListener> listeners = new ArrayList<CollectorListener> ();
}