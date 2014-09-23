package io.crowbar.diagnostic.database;

import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.spectrum.Spectrum;


public final class DiagnosticEntry {
    private final DiagnosticSystem ds;
    private final Spectrum spectrum;
    private final DiagnosticReport dr;

    public DiagnosticSystem getDiagnosticSystem () {
        return ds;
    }

    public Spectrum getSpectrum () {
        return spectrum;
    }

    public DiagnosticReport getDiagnosticReport () {
        return dr;
    }

    public DiagnosticEntry (DiagnosticSystem ds,
                            Spectrum spectrum,
                            DiagnosticReport dr) {
        this.ds = ds;
        this.spectrum = spectrum;
        this.dr = dr;
    }
}