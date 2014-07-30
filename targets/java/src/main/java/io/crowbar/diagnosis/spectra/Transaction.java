package io.crowbar.diagnosis.spectra;

public interface Transaction {
    public int size ();
    public int getCount (int componentId);
    public double getError ();
    public double getConfidence ();
}