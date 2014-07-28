package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class HitVector {
    public class ProbeGroup {
        public final class Probe {
            private int globalId;
            private int localId;
            private int nodeId;
            private ProbeType type;
            private Probe (int globalId,
                           int localId,
                           int nodeId,
                           ProbeType type) {
                this.globalId = globalId;
                this.localId = localId;
                this.nodeId = nodeId;
                this.type = type;
            }

            public boolean getActivation () {
                if (hit_vector == null)
                    return false;

                return hit_vector[localId];
            }

            public int getLocalId () {
                return localId;
            }

            public int getGlobalId () {
                return globalId;
            }

            public int getNodeId () {
                return nodeId;
            }

            public ProbeType getType () {
                return type;
            }

            public void hit () {
                assert hit_vector != null;
                hit_vector[localId] = true;
            }
        }

        private int size = 0;
        private boolean[] hit_vector = null;

        public final Probe register (int globalId,
                                     int nodeId,
                                     ProbeType type) {
            assert hit_vector == null;
            return new Probe(globalId, size++, nodeId, type);
        }

        public final boolean[] get () {
            if (hit_vector == null)
                hit_vector = new boolean[size];

            return hit_vector;
        }

        public final void reset () {
            if (hit_vector == null)
                return;

            for (int j = 0; j < hit_vector.length; j++)
                hit_vector[j] = false;
        }

        private void ProbeGroup () {}
    }


    private List<ProbeGroup.Probe> probes = new ArrayList<ProbeGroup.Probe> ();
    private Map<String, ProbeGroup> groups = new HashMap<String, ProbeGroup> ();


    public Probe registerProbe (String group_name,
                                int nodeId,
                                ProbeType type) {
        ProbeGroup pg = groups.get(group_name);


        if (pg == null) {
            pg = new ProbeGroup();
            groups.put(group_name, pg);
        }

        Probe probe = pg.register(probes.size(), nodeId, type);
        probes.add(probe);

        return probe;
    }

    public final boolean exists (String group_name) {
        return groups.containsKey(group_name);
    }

    public final boolean[] get (String group_name) {
        assert exists(group_name);
        return groups.get(group_name).get();
    }

    public final boolean[] get () {
        boolean[] ret = new boolean[probes.size()];
        int i = 0;

        for (Probe p : probes)
            ret[i++] = p.getActivation();

        return ret;
    }

    public final void hit (int globalId) {
        Probe p = probes.get(globalId);


        p.hit();
    }

    public final void reset () {
        for (Map.Entry<String, ProbeGroup> e : groups.entrySet()) {
            e.getValue().reset();
        }
    }
}