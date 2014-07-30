package io.crowbar.diagnosis.spectra;

public interface Transaction {
    int size ();
    int getCount (int componentId);
    double getError ();
    double getConfidence ();
}