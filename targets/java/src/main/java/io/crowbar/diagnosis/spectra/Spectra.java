package io.crowbar.diagnosis.spectra;

public interface Spectra<A extends Activity, M> extends Iterable<Transaction<A> > {
    int getNumComponents ();
    int getNumTransactions ();
    Transaction<A> getTransaction (int transactionId);
    M getMetadata (int componentId);
}