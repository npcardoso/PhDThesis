package io.crowbar.diagnosis.spectra;

public interface Spectra<A extends Activity,
                         TM extends Metadata,
                         CM extends Metadata>
extends Iterable<Transaction<A, TM> > {
    int getNumTransactions ();
    int getNumComponents ();

    Transaction<A, TM> getTransaction (int transactionId);
    Component<CM> getComponent (int componentId);
}