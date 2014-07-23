package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;


import java.io.Serializable;

public class Messages {
    public interface Message {}

    public static class HelloMessage implements Message, Serializable {
        protected HelloMessage () {}
        public HelloMessage (String id) {
            this.id = id;
        }

        public String id;
    }

    public static class RegisterNodeMessage implements Message, Serializable {
        protected RegisterNodeMessage () {}
        public RegisterNodeMessage (Node node) {
            this.node = node;
        }

        public Node node;
    }

    public static class RegisterProbeMessage implements Message, Serializable {
        protected RegisterProbeMessage () {}
        public RegisterProbeMessage (int probe_id,
                                     int node_id,
                                     ProbeType type) {
            this.probe_id = probe_id;
            this.node_id = node_id;
            this.type = type;
        }

        public int probe_id;
        public int node_id;
        public ProbeType type;
    }

    public static abstract class ProbeMessage implements Message {
        protected ProbeMessage () {}
        public ProbeMessage (int probe_id) {
            this.probe_id = probe_id;
        }

        public int probe_id;
    }

    public static class TransactionStartMessage extends ProbeMessage implements Serializable {
        protected TransactionStartMessage () {}
        TransactionStartMessage (int probe_id) {
            super(probe_id);
        }
    }

    public static class TransactionEndMessage extends ProbeMessage implements Serializable {
        protected TransactionEndMessage () {}
        TransactionEndMessage (int probe_id,
                               boolean[] hit_vector) {
            super(probe_id);
            this.hit_vector = hit_vector;
        }

        public boolean[] hit_vector;
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