package io.crowbar.instrumentation.events;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;

public interface EventListener {
    void registerNode (int nodeId,
                       int parentId,
                       String name,
                       Node.Type type) throws Exception;

    void registerProbe (int probeId,
                        int nodeId,
                        ProbeType type) throws Exception;

    void startTransaction (int probeId) throws Exception;

    void endTransaction (int probeId,
                         boolean[] hitVector) throws Exception;

    void logException (String exceptionClass,
                       String exceptionMessage) throws Exception;

    void oracle (int probeId,
                 double error,
                 double confidence) throws Exception;
}