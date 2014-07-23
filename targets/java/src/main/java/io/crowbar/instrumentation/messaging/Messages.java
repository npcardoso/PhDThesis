package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;


import java.io.Serializable;

public class Messages {
    public interface Message {}

    public static class HelloMessage implements Message, Serializable {
        public String id;

        public HelloMessage (String id) {
            this.id = id;
        }

        @Override
        public String toString () {
            return "[[HelloMessage], id: " + id + "]";
        }

        protected HelloMessage () {}
    }

    public static class ByeMessage implements Message, Serializable {
        @Override
        public String toString () {
            return "[[ByeMessage]]";
        }
    }

    public static class RegisterNodeMessage implements Message, Serializable {
        public Node node;

        public RegisterNodeMessage (Node node) {
            this.node = node;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "node: " + node + "]";
            return ret;
        }

        protected RegisterNodeMessage () {}
    }

    public static class RegisterProbeMessage implements Message, Serializable {
        public int probe_id;
        public int node_id;
        public ProbeType type;

        public RegisterProbeMessage (int probe_id,
                                     int node_id,
                                     ProbeType type) {
            this.probe_id = probe_id;
            this.node_id = node_id;
            this.type = type;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "probe_id: " + probe_id + ", ";
            ret += "node_id: " + node_id + ", ";
            ret += "type: " + type + "";
            return ret;
        }

        protected RegisterProbeMessage () {}
    }

    public static abstract class ProbeMessage implements Message, Serializable {
        public int probe_id;

        public ProbeMessage (int probe_id) {
            this.probe_id = probe_id;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "probe_id: " + probe_id + "]";
            return ret;
        }

        protected ProbeMessage () {}
    }

    public static class TransactionStartMessage extends ProbeMessage implements Serializable {
        TransactionStartMessage (int probe_id) {
            super(probe_id);
        }

        protected TransactionStartMessage () {}
    }

    public static class TransactionEndMessage extends ProbeMessage implements Serializable {
        public boolean[] hit_vector;

        TransactionEndMessage (int probe_id,
                               boolean[] hit_vector) {
            super(probe_id);
            this.hit_vector = hit_vector;
        }

        @Override
        public String toString () {
            String ret = "[[" + this.getClass().getSimpleName() + "]: ";


            ret += "probe_id: " + probe_id + ", ";
            ret += "hit_vector: " + hit_vector + "]";
            return ret;
        }

        protected TransactionEndMessage () {}
    }

    public static class OracleMessage extends ProbeMessage implements Serializable  {
        protected OracleMessage () {}
        OracleMessage (int probe_id,
                       double error,
                       double confidence) {
            super(probe_id);
            this.error = error;
            this.confidence = confidence;
        }

        public double error;
        public double confidence;
    }
}