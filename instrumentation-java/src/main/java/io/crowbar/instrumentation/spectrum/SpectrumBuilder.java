package io.crowbar.instrumentation.spectrum;

import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.instrumentation.events.AbstractEventListener;
import io.crowbar.instrumentation.runtime.Probe;

import java.util.Map;
import java.util.HashMap;


public class SpectrumBuilder extends AbstractEventListener {
    private boolean error = false;

    private Map<Integer,Node> nodeIdTranslation = new HashMap<Integer,Node>();

    private final EditableSpectrum<Hit, TrM> spectrum =
        new EditableSpectrum<Hit, TrM> ();


    public final Spectrum<Hit, TrM> getSpectrum () {
        return spectrum;
    }

    @Override
    public final void registerNode (String name, int id, int parentId) throws Exception {
        assert(nodeIdTranslation.get(id) == null);
        Node n = spectrum.getTree().addNode(name, parentId);
        nodeIdTranslation.put(id, n);

    }

    @Override
    public final void registerProbe (Probe probe) throws Exception {
        Node n = nodeIdTranslation.get(probe.getNodeId());

        assert(n != null);

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
