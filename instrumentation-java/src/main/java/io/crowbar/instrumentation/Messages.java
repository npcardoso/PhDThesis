package io.crowbar.instrumentation;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;

import java.io.Serializable;

public class Messages {
    public interface Message {}

    public static final class HelloMessage implements Message, Serializable {
        private static final long serialVersionUID = 4523313711692005000L;
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
        private static final long serialVersionUID = 3262507873284969712L;

        @Override
        public String toString () {
            return "[[ByeMessage]]";
        }
    }

    public static final class RegisterNodeMessage implements Message, Serializable {
        private static final long serialVersionUID = -1646717811451968968L;
        private final int nodeId;
        private final int parentId;
        private final String name;
        private final Node.Type type;

        public RegisterNodeMessage (int nodeId,
                                    int parentId,
                                    String name,
                                    Node.Type type) {
            this.nodeId = nodeId;
            this.parentId = parentId;
            this.name = name;
            this.type = type;
        }

        public int getNodeId () {
            return nodeId;
        }

        public int getParentId () {
            return parentId;
        }

        public String getName () {
            return name;
        }

        public Node.Type getType () {
            return type;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "id: " + nodeId + ", ";
            ret += "parentId: " + parentId + ", ";
            ret += "name: " + name + "]";
            return ret;
        }

        protected RegisterNodeMessage () {
            this(-1, -1, null, null);
        }
    }

    public abstract static class ProbeMessage implements Message, Serializable {
        private static final long serialVersionUID = 2094257171874796426L;
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

    public static final class RegisterProbeMessage
    extends ProbeMessage
    implements Serializable {
        private static final long serialVersionUID = 8746687288408031667L;
        private final int nodeId;
        private final ProbeType type;

        public RegisterProbeMessage (int probeId,
                                     int nodeId,
                                     ProbeType type) {
            super(probeId);
            this.nodeId = nodeId;
            this.type = type;
        }

        public final int getNodeId () {
            return nodeId;
        }

        public final ProbeType getProbeType () {
            return type;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += getProbeId() + ", ";
            ret += getNodeId() + ", ";
            ret += getProbeType() + "]";
            return ret;
        }

        protected RegisterProbeMessage () {
            this(-1, -1, null);
        }
    }


    public static final class TransactionStartMessage extends ProbeMessage implements Serializable {
        private static final long serialVersionUID = 7459099682879100695L;

        TransactionStartMessage (int probeId) {
            super(probeId);
        }

        protected TransactionStartMessage () {
            this(-1);
        }
    }

    public static final class TransactionEndMessage extends ProbeMessage implements Serializable {
        private static final long serialVersionUID = 5478029092609894069L;
        private final boolean[] hitVector;


        TransactionEndMessage (int probeId,
                               boolean[] hitVector) {
            super(probeId);
            this.hitVector = hitVector;
        }

        public boolean[] getHitVector () {
            return hitVector;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "probeId: " + getProbeId() + ", ";
            ret += "hitVector: " + getHitVector() + "]";
            return ret;
        }

        protected TransactionEndMessage () {
            this(-1, null);
        }
    }

    public static final class LogExceptionMessage implements Message, Serializable {
        private static final long serialVersionUID = 3625078343434529712L;

        private final String exceptionClass;
        private final String exceptionMessage;

        public LogExceptionMessage (String exceptionClass,
                                    String exceptionMessage) {
            this.exceptionClass = exceptionClass;
            this.exceptionMessage = exceptionMessage;
        }

        public String getExceptionClass () {
            return exceptionClass;
        }

        public String getExceptionMessage () {
            return exceptionMessage;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "exceptionClass: " + getExceptionClass() + ", ";
            ret += "exceptionMessage: " + getExceptionMessage() + "]";
            return ret;
        }
    }

    public static final class OracleMessage extends ProbeMessage implements Serializable  {
        private static final long serialVersionUID = -4795516008213040058L;
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