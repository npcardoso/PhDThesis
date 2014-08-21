package io.crowbar.instrumentation.spectra;

import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.events.AbstractEventListener;
import io.crowbar.instrumentation.events.TreeRebuilder;
import io.crowbar.diagnosis.spectra.activity.Hit;
import io.crowbar.diagnosis.spectra.EditableSpectra;
import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Transaction;


public class SpectraBuilder extends AbstractEventListener {
    private boolean error = false;
    private final TreeRebuilder treeRebuilder = new TreeRebuilder();
    private final EditableSpectra<Hit, TrM, CmpM> spectra =
        new EditableSpectra<Hit, TrM, CmpM> ();


    public final Spectra<Hit, TrM, CmpM> getSpectra () {
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


        spectra.setComponent(
            Factory.createComponent(probe.getId(),
                                    new CmpM(probe.getType(), n)));
    }

    @Override
    public final void endTransaction (int probeId,
                                      String exceptionClass,
                                      String exceptionMessage,
                                      boolean[] hitVector) {
        TrM m = new TrM(exceptionClass,
                        exceptionMessage);


        Transaction<Hit, TrM> t =
            Factory.createTransaction(spectra.getTransactionCount(),
                                      hitVector,
                                      error ? 1 : 0,
                                      1, m);


        spectra.setTransaction(t);
        error = false;
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {
        this.error = this.error || (error > 0);
    }
}
