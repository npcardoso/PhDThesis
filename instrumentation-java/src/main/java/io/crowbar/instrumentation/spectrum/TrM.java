package io.crowbar.instrumentation.spectrum;


public final class TrM {
    private final String exceptionClass;
    private final String exceptionMessage;

    public TrM (String exceptionClass,
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