package io.crowbar.diagnosis;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.serializers.HitSpectraSerializer;

import flexjson.JSON;


public final class DiagnosticRequest {
    private final DiagnosticSystem system;
    private final String spectra;


    public DiagnosticRequest (DiagnosticSystem system,
                              Spectra spectra) {
        this.system = system;
        this.spectra = HitSpectraSerializer.serialize(spectra, " ");
    }

    @JSON
    private DiagnosticSystem getSystem () {
        return system;
    }

    @JSON
    private String getSpectra () {
        return spectra;
    }
}