package io.crowbar.instrumentation.messaging;

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

    public static class RegisterMessage implements Message, Serializable {
        protected RegisterMessage () {}
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