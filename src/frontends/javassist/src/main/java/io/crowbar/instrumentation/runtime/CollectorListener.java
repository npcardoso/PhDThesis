package io.crowbar.instrumentation.runtime;

public interface CollectorListener {

	public void startTransaction(Probe p);
	public void endTransaction(Probe p);
}
