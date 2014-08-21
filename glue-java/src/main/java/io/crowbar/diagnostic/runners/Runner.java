package io.crowbar.diagnostic.runners;

import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.spectrum.Spectrum;

public interface Runner {
    DiagnosticReport run (DiagnosticSystem system,
                          Spectrum spectrum) throws ExecutionError;
}
