package io.crowbar.instrumentation.spectrum;

import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.instrumentation.events.AbstractEventListener;
import io.crowbar.instrumentation.events.TreeRebuilder;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Transaction;


public class SpectrumBuilder extends AbstractEventListener {
    private boolean error = false;
    private final TreeRebuilder treeRebuilder = new TreeRebuilder();
    private final EditableSpectrum<Hit, TrM, CmpM> spectrum =
        new EditableSpectrum<Hit, TrM, CmpM> ();


    public final Spectrum<Hit, TrM, CmpM> getSpectrum () {
        return spectrum;
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


        spectrum.setComponent(
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
            Factory.createTransaction(spectrum.getTransactionCount(),
                                      hitVector,
                                      error ? 1 : 0,
                                      1, m);


        spectrum.setTransaction(t);
        error = false;
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {
        this.error = this.error || (error > 0);
    }
}
