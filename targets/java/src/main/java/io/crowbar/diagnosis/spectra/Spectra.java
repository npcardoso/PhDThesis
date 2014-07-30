package io.crowbar.diagnosis.spectra;

public interface Spectra<T, M> extends Iterable<T> {
    int getNumComponents ();
    int getNumTransactions ();
    T getTransaction (int transactionId);
    M getMetadata (int componentId);
}