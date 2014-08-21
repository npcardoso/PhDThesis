package io.crowbar.instrumentation.spectrum;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.diagnostic.spectrum.Metadata;
import io.crowbar.diagnostic.spectrum.Node;

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
