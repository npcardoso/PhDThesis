package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.*;

public interface EventListener {
    public void register (ProbeSet ps);

    public void startTransaction (Probe p);

    public void endTransaction (Probe p,
                                boolean[] hit_vector);

    public void oracle (Probe p,
                        double error,
                        double confidence);
}