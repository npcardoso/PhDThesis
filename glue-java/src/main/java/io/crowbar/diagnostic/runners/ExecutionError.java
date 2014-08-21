package io.crowbar.diagnostic.runners;

public class ExecutionError extends Exception {
    public ExecutionError (String cause) {
        super(cause);
    }
}
