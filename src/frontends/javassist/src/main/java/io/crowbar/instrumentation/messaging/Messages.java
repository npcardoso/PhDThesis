package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.runtime.Probe;

import java.io.Serializable;

public class Messages {
    public interface Message {}

    public static abstract class ProbeMessage implements Message {
        public Probe probe;
        public ProbeMessage (Probe probe) {
            this.probe = probe;
        }
    }

    public static class TransactionStartMessage extends ProbeMessage implements Serializable {
        TransactionStartMessage (Probe probe) {super(probe);}
    }

    public static class TransactionEndMessage extends ProbeMessage implements Serializable {
        TransactionEndMessage (Probe probe) {super(probe);}
    }

    public static class OracleMessage extends ProbeMessage implements Serializable  {
        public double error;
        public double confidence;
        OracleMessage (Probe probe,
                       double error,
                       double confidence) {
            super(probe);
        }
    }
}