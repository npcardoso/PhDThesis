package io.crowbar.diagnosis.spectra;

public interface Spectra<T, M> extends Iterable<T> {
    public int getNumComponents ();
    public int getNumTransactions ();
    public T getTransaction (int transactionId);
    public M getMetadata (int componentId);
}