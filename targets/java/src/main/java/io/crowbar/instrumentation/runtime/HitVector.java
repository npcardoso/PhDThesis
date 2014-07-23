package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class HitVector {
    public class ProbeGroup {
        public class Probe {
            private int global_id;
            private int local_id;
            private int node_id;
            private ProbeType type;
            private Probe (int global_id,
                           int local_id,
                           int node_id,
                           ProbeType type) {
                this.global_id = global_id;
                this.local_id = local_id;
                this.node_id = node_id;
                this.type = type;
            }

            public boolean getActivation () {
                assert hit_vector != null;
                return hit_vector[local_id];
            }

            public int getLocalId () {
                return local_id;
            }

            public int getGlobalId () {
                return global_id;
            }

            public int getNodeId () {
                return node_id;
            }

            public ProbeType getType () {
                return type;
            }

            public void hit () {
                hit_vector[local_id] = true;
            }
        }

        private int size = 0;
        private boolean[] hit_vector = null;

        public Probe register (int global_id,
                               int node_id,
                               ProbeType type) {
            assert hit_vector == null;
            return new Probe(global_id, size++, node_id, type);
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

        private void ProbeGroup () {}
    }


    private List<ProbeGroup.Probe> probes = new ArrayList<ProbeGroup.Probe> ();
    private Map<String, ProbeGroup> groups = new HashMap<String, ProbeGroup> ();


    public Probe registerProbe (String group_name,
                                int node_id,
                                ProbeType type) {
        ProbeGroup pg = groups.get(group_name);


        if (pg == null) {
            pg = new ProbeGroup();
            groups.put(group_name, pg);
        }

        Probe probe = pg.register(probes.size(), node_id, type);
        probes.add(probe);

        return probe;
    }

    public boolean[] get (String group_name) {
        return groups.get(group_name).get();
    }

    public boolean[] get () {
        boolean[] ret = new boolean[probes.size()];
        int i = 0;

        for (Probe p : probes)
            ret[i++] = p.getActivation();

        return ret;
    }

    public void hit (int global_id) {
        Probe p = probes.get(global_id);


        p.hit();
    }

    public void reset () {
        for (Map.Entry<String, ProbeGroup> e : groups.entrySet()) {
            e.getValue().reset();
        }
    }
}