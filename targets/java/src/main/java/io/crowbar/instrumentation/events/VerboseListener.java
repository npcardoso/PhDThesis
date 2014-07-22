package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

public class VerboseListener implements EventListener {
    @Override
    public void registerNode (Node n) {
        System.out.println("!!!!!!!!! Registering Node: " + n +
                           " !!!!!!!!!");
    }

    @Override
    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) {
        System.out.println("!!!!!!!!! Registering Probe: " + probe_id +
                           ", node_id = " + node_id +
                           ", type = " + type + " !!!!!!!!!");
    }

    @Override
    public void startTransaction (int probe_id) {
        System.out.println("!!!!!!!!! transaction start @ " + probe_id + " !!!!!!!!!");
    }

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) {
        System.out.println("!!!!!!!!! transaction end @ " + probe_id + " !!!!!!!!!");

        System.out.print("| ");

        for (boolean b : hit_vector)
            System.out.print(b ? "1 " : "0 ");

        System.out.println("|");
    }

    @Override
    public void oracle (int probe_id,
                        double error,
                        double confidence) {
        System.out.println("!!!!!!!!! collecting oracle @ " + probe_id + " : " + error +
                           ", " + confidence + " !!!!!!!!!");
    }
}