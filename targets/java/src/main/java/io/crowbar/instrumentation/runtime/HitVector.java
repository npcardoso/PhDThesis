package io.crowbar.instrumentation.runtime;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class HitVector {
    public static class ProbeID {
        public int global_id;
        public int group_id, local_id;

        public ProbeID (int global,
                        int group_id,
                        int local_id) {
            this.global_id = global_id;
            this.group_id = group_id;
            this.local_id = local_id;
        }
    }

    public ProbeID getCurrentID () {
        return new ProbeID(num_probes,
                           groups.size(),
                           num_probes_group);
    }

    public void startGroup () {
        if (num_probes_group != 0)
            endGroup();
    }

    public void endGroup () {
        if (num_probes_group == 0)
            return;

        groups.add(new boolean[num_probes_group]);
        num_probes_group = 0;
    }

    public ProbeID register () {
        ProbeID id = getCurrentID();


        num_probes_group++;
        num_probes++;

        return id;
    }

    public boolean[] get (int group_id) {
        return groups.get(group_id);
    }

    public boolean[] get () {
        boolean[] ret = new boolean[num_probes];
        int i = 0;

        for (boolean[] g : groups)
            for (int j = 0; j < g.length; j++)
                ret[i++] = g[j];

        return ret;
    }

    public void reset () {
        for (boolean[] g : groups)
            for (int j = 0; j < g.length; j++)
                g[j] = false;
    }

    private int num_probes = 0;
    private int num_probes_group = 0;
    private List<boolean[]> groups;
    private boolean[] hit_vector = null;
}