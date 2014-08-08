package io.crowbar.instrumentation.spectra;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.diagnosis.spectra.Metadata;

public final class CmpM implements Metadata {
    private final ProbeType type;
    private final Node node;

    public CmpM (ProbeType type,
                 Node node) {
        this.type = type;
        this.node = node;
    }

    public ProbeType getType () {
        return type;
    }

    public Node getNode () {
        return node;
    }
}