package io.crowbar.instrumentation.spectra;

import io.crowbar.diagnosis.spectra.Metadata;

public class TransactionMetadata implements Metadata {
    private final String exceptionClass;
    private final String exceptionMessage;

    public TransactionMetadata (String exceptionClass,
                                String exceptionMessage) {
        this.exceptionClass = exceptionClass;
        this.exceptionMessage = exceptionMessage;
    }

    public String getExceptionClass () {
        return exceptionClass;
    }

    public String getExceptionMessage () {
        return exceptionMessage;
    }
}