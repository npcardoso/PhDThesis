package io.crowbar.instrumentation.runtime;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class HitVector {
    class ProbeGroup {
        class Probe {
            private int id;
            private int node_id;

            Probe (int id,
                   int node_id) {
                this.id = id;
                this.node_id = node_id;
            }

            public boolean getActivation () {
                assert hit_vector != null;
                return hit_vector[id];
            }

            public int getId () { // ! Returns the probe's local id
                return id;
            }
        }

        private int size = 0;
        private boolean[] hit_vector = null;

        public Probe register (int node_id) {
            assert hit_vector == null;
            return new Probe(size++, node_id);
        }

        public boolean[] get () {
            if (hit_vector == null)
                hit_vector = new boolean[size];

            return hit_vector;
        }

        public void reset () {
            for (int j = 0; j < hit_vector.length; j++)
                hit_vector[j] = false;
        }
    }


    private List<ProbeGroup.Probe> probes = new ArrayList<ProbeGroup.Probe> ();
    private Map<String, ProbeGroup> groups = new HashMap<String, ProbeGroup> ();


    public int registerProbe (String group_name,
                              int node_id) {
        ProbeGroup pg = groups.get(group_name);


        if (pg == null) {
            pg = new ProbeGroup();
            groups.put(group_name, pg);
        }

        ProbeGroup.Probe probe = pg.register(node_id);
        return probe.getId();
    }

    public boolean[] get (String group_name) {
        System.out.println(group_name);
        return groups.get(group_name).get();
    }

    public boolean[] get () {
        boolean[] ret = new boolean[probes.size()];
        int i = 0;

        for (ProbeGroup.Probe p : probes)
            ret[i++] = p.getActivation();

        return ret;
    }

    public void reset () {
        for (Map.Entry<String, ProbeGroup> e : groups.entrySet()) {
            e.getValue().reset();
        }
    }
}