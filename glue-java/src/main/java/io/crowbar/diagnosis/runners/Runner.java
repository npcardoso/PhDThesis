package io.crowbar.diagnosis;

import io.crowbar.diagnosis.spectra.Spectra;

public interface Runner {
    public class ExecutionError extends Exception {
        public ExecutionError (String cause) {
            super(cause);
        }
    }

    /*! Gets the server interpretation of the diagnostic system serialization (with imposed cutoffs, etc..) */
    public DiagnosticSystem getServerInterpretation (DiagnosticSystem system) throws ExecutionError;

    public DiagnosticReport run (DiagnosticSystem system,
                                 Spectra spectra) throws ExecutionError;
}
