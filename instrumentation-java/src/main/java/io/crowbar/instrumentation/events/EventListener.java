package io.crowbar.instrumentation.events;

import io.crowbar.diagnostic.spectrum.ProbeType;

public interface EventListener {
    void registerNode (int nodeId,
                       int parentId,
                       String name) throws Exception;

    void registerProbe (int probeId,
                        int nodeId,
                        ProbeType type) throws Exception;

    void startTransaction (int probeId) throws Exception;

    void endTransaction (int probeId,
                         String exceptionClass,
                         String exceptionMessage,
                         boolean[] hitVector) throws Exception;

    void oracle (int probeId,
                 double error,
                 double confidence) throws Exception;
}