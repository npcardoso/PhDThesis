package io.crowbar.instrumentation.runtime;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class ProbeStore {
	public void register (ProbeSet ps) throws Exception {
		try { ps.prepare(probeset_list.size()); } catch (Exception e) { }
		if(!probeset_map.containsKey(ps.getName())) {
			probeset_map.put(ps.getName(), ps);
			probeset_list.add(ps);
			total_probes += ps.size();
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

    public Probe get (int probe_set_id,
                      int probe_id) {
        System.out.println(probe_set_id + ", " + probe_id + "!!!!!!11");
        return probeset_list.get(probe_set_id).get(probe_id);
    }

    public int getNumProbes () {
        return total_probes;
    }

    private Map<String, ProbeSet> probeset_map = new HashMap<String, ProbeSet> ();
    private List<ProbeSet> probeset_list = new ArrayList<ProbeSet> (); // This is needed to maintain serialization order
    private int total_probes = 0;
}