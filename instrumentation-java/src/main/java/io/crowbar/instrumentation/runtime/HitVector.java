package io.crowbar.instrumentation.runtime;

import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


class HitVector {
    private List<HitProbe> probes = new ArrayList<HitProbe> ();
    private Map<String, ProbeGroup> groups = new HashMap<String, ProbeGroup> ();


    public HitProbe registerProbe (String groupName,
                                   int nodeId,
                                   ProbeType type) {
        ProbeGroup pg = groups.get(groupName);


        if (pg == null) {
            pg = new ProbeGroup();
            groups.put(groupName, pg);
        }

        HitProbe probe = pg.register(probes.size(), nodeId, type);
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

        for (HitProbe p : probes) {
            ret[i++] = p.getActivation();
        }

        return ret;
    }

    public final void hit (int globalId) {
        assert globalId >= 0 && globalId < probes.size();

        HitProbe p = probes.get(globalId);
        p.hit();
    }

    public final void reset () {
        for (Map.Entry<String, ProbeGroup> e : groups.entrySet()) {
            e.getValue().reset();
        }
    }
}