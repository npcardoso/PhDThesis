package io.crowbar.diagnosis.runners.messages;

import io.crowbar.diagnosis.DiagnosticReport;
import io.crowbar.diagnosis.DiagnosticSystem;
import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.serializers.HitSpectraSerializer;

import flexjson.JSON;

public class DiagnosticMessages {
    private static final String NAME = "diagnostic";

    public static class Request
        extends io.crowbar.diagnosis.runners.messages.Request {

        private DiagnosticSystem system = null;
        private String spectra = null;

        /*! Used for JSON deserialization */
        private Request () {}

        /*! Used for JSON deserialization */
        private void setSystem (DiagnosticSystem system) {
            this.system = system;
        }

        /*! Used for JSON deserialization */
        private void setSpectra (String spectra) {
            this.spectra = spectra;
        }

        Request (DiagnosticSystem system,
                 Spectra spectra) {
            this.system = system;
            this.spectra = HitSpectraSerializer.serialize(spectra, " ");
        }

        @Override
        public String getType () {
            return NAME;
        }

        @JSON
        public DiagnosticSystem getSystem () {
            return system;
        }

        @JSON
        public String getSpectra () {
            return spectra;
        }
    }

    public static class Response
        extends io.crowbar.diagnosis.runners.messages.Response {

        private DiagnosticSystem system = null;
        private DiagnosticReport report = null;

        /*! Used for JSON deserialization */
        private Response () {}

        /*! Used for JSON deserialization */
        private void setSystem (DiagnosticSystem system) {
            this.system = system;
        }

        /*! Used for JSON deserialization */
        private void setReport (DiagnosticReport report) {
            this.report = report;
        }

        Response (DiagnosticSystem system,
                  DiagnosticReport report) {
            this.system = system;
            this.report = report;
        }

        @Override
        public String getType () {
            return NAME;
        }


        @JSON
        public DiagnosticSystem getSystem () {
            return system;
        }

        @JSON
        public DiagnosticReport getReport() {
            return report;
        }
    }


    public static DiagnosticMessages.Request issueRequest (DiagnosticSystem system,
                                                           Spectra spectra) {
        return new Request(system, spectra);
    }

    public static DiagnosticMessages.Response issueResponse (DiagnosticSystem system,
                                                             DiagnosticReport report) {
        return new Response(system, report);
    }

    static {
        Messages.registerRequest(NAME, DiagnosticMessages.Request.class);
        Messages.registerResponse(NAME, DiagnosticMessages.Response.class);
    }
}
