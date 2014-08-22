package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.diagnostic.spectrum.Node;

public interface EventListener {
    void registerNode (String name, int id, int parentId) throws Exception;

    void registerProbe (Probe p) throws Exception;

    void startTransaction (int probeId) throws Exception;

    void endTransaction (int probeId,
                         String exceptionClass,
                         String exceptionMessage,
                         boolean[] hitVector) throws Exception;

    void oracle (int probeId,
                 double error,
                 double confidence) throws Exception;
}
