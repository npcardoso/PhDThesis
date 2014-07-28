package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class HitVector {
    public final class ProbeGroup {
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
                if (hitVector == null)
                    return false;

                return hitVector[localId];
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
                assert hitVector != null;
                hitVector[localId] = true;
            }
        }

        private int size = 0;
        private boolean[] hitVector = null;

        public Probe register (int globalId,
                                     int nodeId,
                                     ProbeType type) {
            assert hitVector == null;
            return new Probe(globalId, size++, nodeId, type);
        }

        public boolean[] get () {
            if (hitVector == null)
                hitVector = new boolean[size];

            return hitVector;
        }

        public void reset () {
            if (hitVector == null)
                return;

            for (int j = 0; j < hitVector.length; j++) {
                hitVector[j] = false;
            }
        }

        private ProbeGroup () {}
    }


    private List<ProbeGroup.Probe> probes = new ArrayList<ProbeGroup.Probe> ();
    private Map<String, ProbeGroup> groups = new HashMap<String, ProbeGroup> ();


    public Probe registerProbe (String groupName,
                                int nodeId,
                                ProbeType type) {
        ProbeGroup pg = groups.get(groupName);


        if (pg == null) {
            pg = new ProbeGroup();
            groups.put(groupName, pg);
        }

        Probe probe = pg.register(probes.size(), nodeId, type);
        probes.add(probe);

        return probe;
    }

    public final boolean exists (String groupName) {
        return groups.containsKey(groupName);
    }

    public final boolean[] get (String groupName) {
        assert exists(groupName);
        return groups.get(groupName).get();
    }

    public final boolean[] get () {
        boolean[] ret = new boolean[probes.size()];
        int i = 0;

        for (Probe p : probes) {
            ret[i++] = p.getActivation();
        }

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