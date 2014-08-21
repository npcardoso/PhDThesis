package io.crowbar.diagnosis.runners;

import io.crowbar.diagnosis.DiagnosticReport;
import io.crowbar.diagnosis.DiagnosticSystem;
import io.crowbar.diagnosis.spectra.Spectra;

public interface Runner {
    DiagnosticReport run (DiagnosticSystem system,
                          Spectra spectra) throws ExecutionError;
}
