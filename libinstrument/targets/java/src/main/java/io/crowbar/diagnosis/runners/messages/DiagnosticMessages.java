package io.crowbar.diagnosis.runners.messages;

import io.crowbar.diagnosis.DiagnosticSystem;
import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.serializers.HitSpectraSerializer;

import flexjson.JSON;

public class DiagnosticMessages {
    private static final String NAME = "diagnostic";

    private static class RequestPvt extends Request {
        private DiagnosticSystem system = null;
        private String spectra = null;

        /*! Used for JSON deserialization */
        private RequestPvt () {}

        /*! Used for JSON deserialization */
        private void setSystem (DiagnosticSystem system) {
            this.system = system;
        }

        /*! Used for JSON deserialization */
        private void setSpectra (String spectra) {
            this.spectra = spectra;
        }

        @Override
        public String getType () {
            return NAME;
        }

        public RequestPvt (DiagnosticSystem system,
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

    public static Request issueRequest (DiagnosticSystem system,
                                        Spectra spectra) {
        return new RequestPvt(system,
                              spectra);
    }

    public static Response issueResponse () {
        return null;
    }

    static {
        Messages.registerRequest(NAME, RequestPvt.class);
    }
}