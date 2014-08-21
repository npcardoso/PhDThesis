package io.crowbar.diagnostic.runners.messages;

import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.serializers.HitSpectrumSerializer;

import flexjson.JSON;

public class DiagnosticMessages {
    private static final String NAME = "diagnostic";

    public static final class Request
        extends io.crowbar.diagnostic.runners.messages.Request {

        private DiagnosticSystem system = null;
        private String spectrum = null;

        /*! Used for JSON deserialization */
        private Request () {}

        /*! Used for JSON deserialization */
        private void setSystem (DiagnosticSystem system) {
            this.system = system;
        }

        /*! Used for JSON deserialization */
        private void setSpectrum (String spectrum) {
            this.spectrum = spectrum;
        }

        Request (DiagnosticSystem system,
                 Spectrum spectrum) {
            this.system = system;
            this.spectrum = HitSpectrumSerializer.serialize(spectrum, " ");
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
        public String getSpectrum () {
            return spectrum;
        }
    }

    public static final class Response
        extends io.crowbar.diagnostic.runners.messages.Response {

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
                                                           Spectrum spectrum) {
        return new Request(system, spectrum);
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
