package io.crowbar.instrumentation.spectra;

import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.events.AbstractEventListener;
import io.crowbar.instrumentation.events.TreeRebuilder;


public class SpectraBuilder extends AbstractEventListener {
    private boolean error = false;
    private final TreeRebuilder treeRebuilder = new TreeRebuilder();
    private final EditableSpectra spectra = new EditableSpectra();


    public final Spectra getSpectra () {
        return spectra;
    }

    public final Tree getTree () {
        return treeRebuilder.getTree();
    }

    @Override
    public final void registerNode (Node node) throws Exception {
        treeRebuilder.registerNode(node);
    }

    @Override
    public final void registerProbe (Probe probe) throws Exception {
        Node n = treeRebuilder.getTree().getNode(probe.getNodeId());


        spectra.setComponent(probe.getId(),
                             new Component(
                                 new ComponentMetadata(probe.getType(), n)));
    }

    @Override
    public final void endTransaction (int probeId,
                                      String exceptionClass,
                                      String exceptionMessage,
                                      boolean[] hitVector) {
        TransactionMetadata m = new TransactionMetadata(exceptionClass,
                                                        exceptionMessage);
        HitTransaction t = new HitTransaction(hitVector,
                                              error ? 1 : 0, 1,
                                              m);


        spectra.appendTransaction(t);
        error = false;
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {
        this.error = this.error || (error > 0);
    }
}