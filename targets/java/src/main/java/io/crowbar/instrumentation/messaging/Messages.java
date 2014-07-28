package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;


import java.io.Serializable;

public class Messages {
    public interface Message {}

    public static class HelloMessage implements Message, Serializable {
        private String id;

        public HelloMessage (String id) {
            this.id = id;
        }

        public final String getId() {
        	return id;
        }
        
        @Override
        public final String toString () {
            return "[[HelloMessage], id: " + id + "]";
        }

        protected HelloMessage () {}
    }

    public static class ByeMessage implements Message, Serializable {
        @Override
        public final String toString () {
            return "[[ByeMessage]]";
        }
    }

    public static class RegisterNodeMessage implements Message, Serializable {
        private Node node;

        public RegisterNodeMessage (Node node) {
            this.node = new Node(node);
        }
        
        public final Node getNode() {
        	return node;
        }

        @Override
        public final String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "node: " + node + "]";
            return ret;
        }

        protected RegisterNodeMessage () {}
    }

    public static class RegisterProbeMessage implements Message, Serializable {
        private int probeId;
        private int nodeId;
        private ProbeType type;

        public RegisterProbeMessage (int probeId,
                                     int nodeId,
                                     ProbeType type) {
            this.probeId = probeId;
            this.nodeId = nodeId;
            this.type = type;
        }

        public final int getProbeId() {
        	return probeId;
        }
        
        public final int getNodeId() {
        	return nodeId;
        }
        
        public final ProbeType getType() {
        	return type;
        }
        
        @Override
        public final String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "probe_id: " + probeId + ", ";
            ret += "node_id: " + nodeId + ", ";
            ret += "type: " + type + "";
            return ret;
        }

        protected RegisterProbeMessage () {}
    }

    public abstract static class ProbeMessage implements Message, Serializable {
        private int probeId;

        public ProbeMessage (int probeId) {
            this.probeId = probeId;
        }
        
        public final int getProbeId() {
        	return probeId;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "probe_id: " + probeId + "]";
            return ret;
        }

        protected ProbeMessage () {}
    }

    public static class TransactionStartMessage extends ProbeMessage implements Serializable {
        TransactionStartMessage (int probeId) {
            super(probeId);
        }

        protected TransactionStartMessage () {}
    }

    public static class TransactionEndMessage extends ProbeMessage implements Serializable {
        private boolean[] hitVector;

        TransactionEndMessage (int probeId,
                               boolean[] hitVector) {
            super(probeId);
            this.hitVector = hitVector;
        }

        public final boolean[] getHitVector() {
        	return hitVector;
        }
        
        @Override
        public final String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "probe_id: " + getProbeId() + ", ";
            ret += "hit_vector: " + hitVector + "]";
            return ret;
        }

        protected TransactionEndMessage () {}
    }

    public static class OracleMessage extends ProbeMessage implements Serializable  {
    	private double error;
        private double confidence;
        
    	protected OracleMessage () {}
        OracleMessage (int probeId,
                       double error,
                       double confidence) {
            super(probeId);
            this.error = error;
            this.confidence = confidence;
        }
        
        public final double getError() {
        	return error;
        }
        
        public final double getConfidence() {
        	return confidence;
        }
    }
}