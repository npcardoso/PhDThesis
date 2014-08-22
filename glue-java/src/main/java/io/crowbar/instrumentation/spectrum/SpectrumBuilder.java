package io.crowbar.instrumentation.spectrum;

import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.events.AbstractEventListener;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Tree;


public class SpectrumBuilder extends AbstractEventListener {
    private boolean error = false;
    private final EditableSpectrum<Hit, TrM> spectrum =
        new EditableSpectrum<Hit, TrM> ();


    public final Spectrum<Hit, TrM> getSpectrum () {
        return spectrum;
    }

    @Override
    public final void registerNode (String name, int id, int parentId) throws Exception {
        // FIXME: maintain a translation map for ids
        spectrum.getTree().addNode(name, parentId);
    }

    @Override
    public final void registerProbe (Probe probe) throws Exception {
        Node n = spectrum.getTree().getNode(probe.getNodeId());
        if(n == null) //! \todo handle this case properly
            n = spectrum.getTree().getRoot();
        spectrum.setComponent(probe.getId(), probe.getType(), n);
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
