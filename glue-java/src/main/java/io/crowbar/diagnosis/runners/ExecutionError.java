package io.crowbar.diagnosis.runners;

public class ExecutionError extends Exception {
    public ExecutionError (String cause) {
        super(cause);
    }
}
