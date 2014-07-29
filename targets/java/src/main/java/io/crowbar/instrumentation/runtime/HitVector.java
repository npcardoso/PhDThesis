package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.ProbeGroup.Probe;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


class HitVector {
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