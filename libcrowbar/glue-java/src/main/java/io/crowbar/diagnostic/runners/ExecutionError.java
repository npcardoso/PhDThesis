package io.crowbar.diagnostic.runners;

public class ExecutionError extends Exception {
    private static final long serialVersionUID = -1799920163689207369L;

    public ExecutionError (String cause) {
        super(cause);
    }
}