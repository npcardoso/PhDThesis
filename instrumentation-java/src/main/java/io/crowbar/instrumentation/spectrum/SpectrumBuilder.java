package io.crowbar.instrumentation.spectrum;

import io.crowbar.diagnostic.spectrum.EditableSpectrum;
import io.crowbar.diagnostic.spectrum.HitTransaction;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.instrumentation.events.AbstractEventListener;

import java.util.HashMap;
import java.util.Map;


public final class SpectrumBuilder extends AbstractEventListener {
    private boolean error = false;
    private String exceptionClass = null;
    private String exceptionMessage = null;

    private Map<Integer, Node> nodeIdTranslation = new HashMap<Integer, Node> ();

    private final EditableSpectrum spectrum =
        new EditableSpectrum();


    public Spectrum getSpectrum () {
        return spectrum;
    }

    private void reset () {
        exceptionClass = null;
        exceptionMessage = null;
        error = false;
    }

    @Override
    public void registerNode (int nodeId,
                              int parentId,
                              String name,
                              Node.Type type) throws Exception {
        assert (nodeIdTranslation.get(nodeId) == null);
        Node n = spectrum.getTree().addNode(name, type, parentId);
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
                                      boolean[] hitVector) {
        Transaction t =
            new HitTransactionWithException(
                spectrum.getTransactionCount(),
                hitVector,
                error ? 1 : 0,
                1,
                exceptionClass,
                exceptionMessage);


        System.out.println(t);

        spectrum.setTransaction(t);
        reset();
    }

    @Override
    public void logException (String exceptionClass,
                              String exceptionMessage) throws Exception {
        this.exceptionClass = exceptionClass;
        this.exceptionMessage = exceptionMessage;
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {
        this.error = this.error || (error > 0);
    }
}