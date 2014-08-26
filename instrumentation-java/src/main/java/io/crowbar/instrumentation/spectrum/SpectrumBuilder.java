package io.crowbar.instrumentation.spectrum;

import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.instrumentation.events.AbstractEventListener;

import java.util.HashMap;
import java.util.Map;


public final class SpectrumBuilder extends AbstractEventListener {
    private boolean error = false;

    private Map<Integer, Node> nodeIdTranslation = new HashMap<Integer, Node> ();

    private final EditableSpectrum<Hit, TrM> spectrum =
        new EditableSpectrum<Hit, TrM> ();


    public Spectrum<Hit, TrM> getSpectrum () {
        return spectrum;
    }

    @Override
    public void registerNode (int nodeId,
                              int parentId,
                              String name) throws Exception {
        assert (nodeIdTranslation.get(nodeId) == null);
        Node n = spectrum.getTree().addNode(name, parentId);
        nodeIdTranslation.put(nodeId, n);
    }

    @Override
    public void registerProbe (int probeId,
                               int nodeId,
                               ProbeType type) throws Exception {
        Node n = nodeIdTranslation.get(nodeId);


        assert (n != null);

        spectrum.setProbe(probeId, type, n);
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