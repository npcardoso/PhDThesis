package io.crowbar.diagnosis.runners;

import io.crowbar.diagnosis.DiagnosticReport;
import io.crowbar.diagnosis.DiagnosticSystem;
import io.crowbar.diagnosis.Runner;
import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.runners.messages.DiagnosticMessages;
import io.crowbar.diagnosis.runners.messages.Messages;

import com.sun.jna.Library;
import com.sun.jna.Pointer;
import com.sun.jna.Native;
import com.sun.jna.ptr.PointerByReference;
import flexjson.JSONDeserializer;

public class JNARunner implements Runner {
    public interface DiagnosticNative extends Library {
        void run (String request, PointerByReference response);
        void cleanup (Pointer response);
    }

    @Override
    public DiagnosticSystem getServerInterpretation (DiagnosticSystem system) throws ExecutionError {
        return system;
    }

    @Override
    public final DiagnosticReport run (DiagnosticSystem system,
                                       Spectra spectra) throws ExecutionError {
        String jsonRequest = Messages.serialize(DiagnosticMessages.issueRequest(system, spectra));
        String jsonResponse = null;

        try {
            DiagnosticNative libdiag = (DiagnosticNative) Native.loadLibrary("diag", DiagnosticNative.class);
            PointerByReference ptrRef = new PointerByReference();
            libdiag.run(jsonRequest, ptrRef);
            Pointer p = ptrRef.getValue();
            jsonResponse = p.getString(0);
            libdiag.cleanup(p);
        } catch (Throwable e) {
            e.printStackTrace();
        }

        JSONDeserializer<DiagnosticReport> dejson = new JSONDeserializer<DiagnosticReport>();
        return dejson.deserialize(jsonResponse, new DiagnosticReport.JSONObjectFactory());
    }
}
