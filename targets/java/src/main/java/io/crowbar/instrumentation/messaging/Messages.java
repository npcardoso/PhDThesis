package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.ProbeSet;

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
        public RegisterMessage (ProbeSet probe_set) throws ProbeSet.NotPreparedException {
            if (!probe_set.isPrepared())
                throw new ProbeSet.NotPreparedException();

            this.probe_set = probe_set;
        }

        public ProbeSet probe_set;
    }

    public static abstract class ProbeMessage implements Message {
        protected ProbeMessage () {}
        public ProbeMessage (Probe probe) throws ProbeSet.NotPreparedException {
            this.probe_set_id = probe.getProbeSet().getId();
            this.probe_id = probe.getId();
        }

        public int probe_set_id;
        public int probe_id;
    }

    public static class TransactionStartMessage extends ProbeMessage implements Serializable {
        protected TransactionStartMessage () {}
        TransactionStartMessage (Probe probe) throws ProbeSet.NotPreparedException {
            super(probe);
        }
    }

    public static class TransactionEndMessage extends ProbeMessage implements Serializable {
        protected TransactionEndMessage () {}
        TransactionEndMessage (Probe probe,
                               boolean[] hit_vector) throws ProbeSet.NotPreparedException {
            super(probe);
            this.hit_vector = hit_vector;
        }

        public boolean[] hit_vector;
    }

    public static class OracleMessage extends ProbeMessage implements Serializable  {
        protected OracleMessage () {}
        OracleMessage (Probe probe,
                       double error,
                       double confidence) throws ProbeSet.NotPreparedException {
            super(probe);
            this.error = error;
            this.confidence = confidence;
        }

        public double error;
        public double confidence;
    }
}