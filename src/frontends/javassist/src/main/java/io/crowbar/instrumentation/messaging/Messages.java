package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.runtime.Probe;

import java.io.Serializable;

public class Messages {
    public static abstract class Message {
        protected Message () {}
        public Message (String client_id) {
            this.client_id = client_id;
        }

        String client_id;
    }

    public static abstract class ProbeMessage extends Message {
        protected ProbeMessage () {}
        public ProbeMessage (String client_id,
                             Probe probe) {
            super(client_id);
            this.probe = probe;
        }

        public Probe probe;
    }

    public static class TransactionStartMessage extends ProbeMessage implements Serializable {
        protected TransactionStartMessage () {}
        TransactionStartMessage (String client_id,
                                 Probe probe) {
            super(client_id, probe);
        }
    }

    public static class TransactionEndMessage extends ProbeMessage implements Serializable {
        protected TransactionEndMessage () {}
        TransactionEndMessage (String client_id,
                               Probe probe,
                               boolean[] hit_vector) {
            super(client_id, probe);
            this.hit_vector = hit_vector;
        }

        public boolean[] hit_vector;
    }

    public static class OracleMessage extends ProbeMessage implements Serializable  {
        protected OracleMessage () {}
        OracleMessage (String client_id,
                       Probe probe,
                       double error,
                       double confidence) {
            super(client_id, probe);
            this.error = error;
            this.confidence = confidence;
        }

        public double error;
        public double confidence;
    }
}