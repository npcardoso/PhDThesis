package io.crowbar.instrumentation.events;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;

import java.io.PrintStream;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


public class VerboseListener
implements EventListener {
    private static final Logger logger = LogManager.getLogger(VerboseListener.class);

    private final Level level;

    private boolean registerNode = true;
    private boolean registerProbe = true;
    private boolean startTransaction = true;
    private boolean endTransaction = true;
    private boolean logException = true;
    private boolean oracle = true;

    public VerboseListener () {
        this(Level.DEBUG);
    }

    public VerboseListener (Level level) {
        this.level = level;
    }

    public final void enableRegisterNode (boolean enable) {
        registerNode = enable;
    }

    public final void enableRegisterProbe (boolean enable) {
        registerProbe = enable;
    }

    public final void enableStartTransaction (boolean enable) {
        startTransaction = enable;
    }

    public final void enableEndTransaction (boolean enable) {
        endTransaction = enable;
    }

    public final void enableLogException (boolean enable) {
        logException = enable;
    }

    public final void enableOracle (boolean enable) {
        oracle = enable;
    }

    @Override
    public final void registerNode (int nodeId,
                                    int parentId,
                                    String name,
                                    Node.Type type) throws Exception {
        if (!registerNode) return;

        logger.log(level, "Registering Node (nId: {}, name: '{}', pnId: {})", nodeId, name, parentId);
    }

    @Override

    public void registerProbe (int probeId,
                               int nodeId,
                               ProbeType type) throws Exception {
        if (!registerProbe) return;

        logger.log(level, "Registering Probe (pId:{}, nId: {}, type: '{}')", probeId, nodeId, type);
    }

    @Override
    public final void startTransaction (int probeId) throws Exception {
        if (!startTransaction) return;

        logger.log(level, "Registering Transaction Start (pId: {})", probeId);
    }

    @Override
    public final void endTransaction (int probeId,
                                      boolean[] hitVector) throws Exception {
        if (!endTransaction) return;

        logger.log(level, "Registering Transaction End (pId: {})", probeId);
    }

    @Override
    public void logException (String exceptionClass,
                              String exceptionMessage) throws Exception {
        if (!logException) return;

        logger.log(level, "Logging Exception (cls: {}, msg: '{}')", exceptionClass, exceptionMessage);
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) throws Exception {
        if (!oracle) return;

        logger.log(level, "Logging Oracle (pId: {}, err: {}, conf: {})", probeId, error, confidence);
    }
}