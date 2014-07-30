package io.crowbar.instrumentation.spectra;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Node;

public final class Metadata {
    private final ProbeType type;
    private final Node node;

    public Metadata (ProbeType type,
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