package io.crowbar.instrumentation.events;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;

import static org.junit.Assert.assertEquals;

public final class EventListenerMocks {
    public static class EventListenerChecks {
        public String nodeName;
        public int parentNodeId;

        public String probeGroupName;
        public int probeNodeId;
        public ProbeType probeType;

        public int startTransactionId;

        public int endTransactionId;
        public String endTransactionExceptionClass;
        public String endTransactionExceptionMessage;

        public int oracleId;
        public double oracleError;
        public double oracleConfidence;
    }

    public static abstract class TestableEventListener extends AbstractEventListener {
        protected boolean triggered;
        protected EventListenerChecks checks;

        public TestableEventListener (EventListenerChecks checks) {
            this.checks = checks;
            resetTrigger();
        }

        public boolean wasTriggered () {
            return triggered;
        }

        public void resetTrigger () {
            triggered = false;
        }
    }

    public static final class TestableRegisterNodeListener extends TestableEventListener {
        public TestableRegisterNodeListener (EventListenerChecks eventListenerChecks) {
            super(eventListenerChecks);
        }

        @Override
        public void registerNode (int nodeId,
                                  int parentId,
                                  String name,
                                  Node.Type type) throws Exception {
            assertEquals(checks.nodeName, name);
            assertEquals(checks.parentNodeId, parentId);
            triggered = true;
        }
    }

    public static final class TestableRegisterProbeListener extends TestableEventListener {
        public TestableRegisterProbeListener (EventListenerChecks eventListenerChecks) {
            super(eventListenerChecks);
        }

        @Override
        public void registerProbe (int probeId,
                                   int nodeId,
                                   ProbeType type) throws Exception {
            assertEquals(checks.probeNodeId, probeId);
            assertEquals(checks.probeType, type);
            triggered = true;
        }
    }

    public static final class TestableTransactionStartListener extends TestableEventListener {
        public TestableTransactionStartListener (EventListenerChecks eventListenerChecks) {
            super(eventListenerChecks);
        }

        @Override
        public void startTransaction (int probeId) {
            assertEquals(checks.startTransactionId, probeId);
            triggered = true;
        }
    }

    public static final class TestableTransactionEndListener extends TestableEventListener {
        public TestableTransactionEndListener (EventListenerChecks eventListenerChecks) {
            super(eventListenerChecks);
        }

        @Override
        public void endTransaction (int probeId,
                                    String exceptionClass,
                                    String exceptionMessage,
                                    boolean[] hitVector) {
            assertEquals(checks.endTransactionId, probeId);
            assertEquals(checks.endTransactionExceptionClass, exceptionClass);
            assertEquals(checks.endTransactionExceptionMessage, exceptionMessage);
            triggered = true;
        }
    }

    public static final class OracleListener extends TestableEventListener {
        public OracleListener (EventListenerChecks checks) {
            super(checks);
        }

        @Override
        public void oracle (int probeId,
                            double error,
                            double confidence) throws Exception {
            assertEquals(checks.oracleId, probeId);
            assertEquals(checks.oracleError, error, 1e-6);
            assertEquals(checks.oracleConfidence, confidence, 1e-6);
            triggered = true;
        }
    }
}