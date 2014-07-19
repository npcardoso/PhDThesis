package io.crowbar.instrumentation.runtime;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class ProbeStore {
    public static class AlreadyRegistered extends Exception {
        AlreadyRegistered (String reason) {super(reason);}
    }

    public void register (ProbeSet ps) throws ProbeSet.NotPreparedException, AlreadyRegistered {
        ProbeSet ps_name = by_name.get(ps.getName());
        ProbeSet ps_id = by_id.get(ps.getId());


        if (ps_name == null && ps_id == null) {
            by_name.put(ps.getName(), ps);
            by_id.put(ps.getId(), ps);
            total_probes += ps.size();
            return;
        }

        if (ps_name == null)
            throw new AlreadyRegistered("'" + ps.getId() + "' was already registered for class '" + ps_id.getName() + "' ");

        if (ps_id == null)
            throw new AlreadyRegistered("'" + ps.getName() + "' was already registered with id '" + ps_name.getId() + "' ");


        throw new AlreadyRegistered("'" + ps.getName() + "' was already registered with id '" + ps_name.getId() +
                                    "' and '" + ps.getId() + "' was already registered for class '" + ps_id.getName() + "' ");
    }

    public ProbeSet get (String name) {
        return by_name.get(name);
    }

    public ProbeSet get (int probe_set_id) {
        return by_id.get(probe_set_id);
    }

    public Probe get (int probe_set_id,
                      int probe_id) {
        return by_id.get(probe_set_id).get(probe_id);
    }

    public int getNumProbes () {
        return total_probes;
    }

    public int getNumProbesSets () {
        return by_id.size();
    }

    private Map<String, ProbeSet> by_name = new HashMap<String, ProbeSet> ();
    private Map<Integer, ProbeSet> by_id = new HashMap<Integer, ProbeSet> ();
    private int total_probes = 0;
}