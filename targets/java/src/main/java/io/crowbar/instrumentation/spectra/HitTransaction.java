package io.crowbar.instrumentation.spectra;

public class HitTransaction extends io.crowbar.diagnosis.spectra.HitTransaction {
    private final String exception;

    public HitTransaction (boolean[] hitVector,
                           double error,
                           double confidence,
                           String exception) {
        super(hitVector, error, confidence);
        this.exception = exception;
    }

    public final String getException () {
        return exception;
    }
}