package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.runtime.Probe;

public class Messages {

	public interface Message {}
	public static abstract class ProbeMessage implements Message {
		public Probe probe;
		public ProbeMessage setProbe(Probe probe) {
			this.probe = probe;
			return this;
		}
	}
	
	public static class TransactionStartMessage extends ProbeMessage { }
	public static class TransactionEndMessage extends ProbeMessage { }
}
