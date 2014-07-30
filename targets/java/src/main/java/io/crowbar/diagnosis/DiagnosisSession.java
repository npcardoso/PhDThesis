package io.crowbar.diagnosis;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.events.MultiListener;
import io.crowbar.instrumentation.events.SpectraBuilder;
import io.crowbar.instrumentation.events.TreeRebuilder;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.spectra.Spectra;


import java.util.ArrayList;


public class DiagnosisSession implements EventListener {
    private final MultiListener listeners = new MultiListener();
    private final SpectraBuilder spectraBuilder;
    private final TreeRebuilder treeRebuilder;
    private final ArrayList<Probe> probes = new ArrayList<Probe> ();


    public DiagnosisSession () {
        this(new SpectraBuilder(),
             new TreeRebuilder());
    }

    public DiagnosisSession (SpectraBuilder spectraBuilder,
                             TreeRebuilder treeRebuilder) {
        this.spectraBuilder = spectraBuilder;
        listeners.add(spectraBuilder);
        this.treeRebuilder = treeRebuilder;
        listeners.add(treeRebuilder);
    }

    public final Spectra getSpectra () {
        return spectraBuilder.getSpectra();
    }

    public final Tree getTree () {
        return treeRebuilder.getTree();
    }

    public final Node getNode (int probeId) {
        Probe p = probes.get(probeId);
        Tree t = treeRebuilder.getTree();


        return t.getNode(p.getNodeId());
    }

    @Override
    public final void registerNode (Node n) throws Exception {
        listeners.registerNode(n);
    }

    @Override
    public final void registerProbe (Probe probe) throws Exception {
        listeners.registerProbe(probe);
    }

    @Override
    public final void startTransaction (int probeId) throws Exception {
        listeners.startTransaction(probeId);
    }

    @Override
    public final void endTransaction (int probeId,
                                      String exception,
                                      boolean[] hitVector) throws Exception {
        listeners.endTransaction(probeId, exception, hitVector);
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) throws Exception {
        listeners.oracle(probeId, error, confidence);
    }
}