package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;


import java.io.Serializable;

public class Messages {
    public interface Message {}

    public static final class HelloMessage implements Message, Serializable {
        private final String id;

        public HelloMessage (String id) {
            this.id = id;
        }

        public String getId () {
            return id;
        }

        @Override
        public String toString () {
            return "[[HelloMessage], id: " + id + "]";
        }

        protected HelloMessage () {
            this(null);
        }
    }

    public static final class ByeMessage implements Message, Serializable {
        @Override
        public String toString () {
            return "[[ByeMessage]]";
        }
    }

    public static final class RegisterNodeMessage implements Message, Serializable {
        private final Node node;

        public RegisterNodeMessage (Node node) {
            this.node = new Node(node);
        }

        public Node getNode () {
            return node;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "node: " + node + "]";
            return ret;
        }

        protected RegisterNodeMessage () {
            this(null);
        }
    }

    public static final class RegisterProbeMessage implements Message, Serializable {
        private final Probe probe;
        public RegisterProbeMessage (Probe probe) {
            this.probe = new Probe(probe);
        }

        public Probe getProbe () {
            return probe;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += probe + "]";
            return ret;
        }

        protected RegisterProbeMessage () {
            this(null);
        }
    }

    public abstract static class ProbeMessage implements Message, Serializable {
        private final int probeId;

        public ProbeMessage (int probeId) {
            this.probeId = probeId;
        }

        public final int getProbeId () {
            return probeId;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "probe_id: " + probeId + "]";
            return ret;
        }

        protected ProbeMessage () {
            this(-1);
        }
    }

    public static final class TransactionStartMessage extends ProbeMessage implements Serializable {
        TransactionStartMessage (int probeId) {
            super(probeId);
        }

        protected TransactionStartMessage () {
            this(-1);
        }
    }

    public static final class TransactionEndMessage extends ProbeMessage implements Serializable {
        private final String exceptionClass;
        private final String exceptionMessage;
        private final boolean[] hitVector;


        TransactionEndMessage (int probeId,
                               String exceptionClass,
                               String exceptionMessage,
                               boolean[] hitVector) {
            super(probeId);
            this.exceptionClass = exceptionClass;
            this.exceptionMessage = exceptionMessage;
            this.hitVector = hitVector;
        }

        public String getExceptionClass () {
            return exceptionClass;
        }

        public String getExceptionMessage () {
            return exceptionMessage;
        }

        public boolean[] getHitVector () {
            return hitVector;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "probeId: " + getProbeId() + ", ";
            ret += "exceptionClass: " + getExceptionClass() + ", ";
            ret += "exceptionMessage: " + getExceptionMessage() + ", ";
            ret += "hitVector: " + getHitVector() + "]";
            return ret;
        }

        protected TransactionEndMessage () {
            this(-1, null, null, null);
        }
    }

    public static final class OracleMessage extends ProbeMessage implements Serializable  {
        private final double error;
        private final double confidence;

        OracleMessage (int probeId,
                       double error,
                       double confidence) {
            super(probeId);
            this.error = error;
            this.confidence = confidence;
        }

        public double getError () {
            return error;
        }

        public double getConfidence () {
            return confidence;
        }

        protected OracleMessage () {
            this(-1, -1, -1);
        }
    }
}