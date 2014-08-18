package io.crowbar.diagnosis.runners;

import io.crowbar.diagnosis.DiagnosticSystem;
import io.crowbar.diagnosis.DiagnosticSystemRunner;
import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.runners.messages.DiagnosticMessages;
import io.crowbar.diagnosis.runners.messages.Messages;

import com.sun.jna.Library;
import com.sun.jna.Pointer;
import com.sun.jna.Native;
import com.sun.jna.ptr.PointerByReference;

public class JNARunner implements DiagnosticSystemRunner {
    public interface DiagnosticNative extends Library {
        void run (String request, PointerByReference response);
        void cleanup (Pointer response);
    }

    @Override
    public DiagnosticSystem getServerInterpretation (DiagnosticSystem system) throws ExecutionError {
        return system;
    }

    @Override
    public final void run (DiagnosticSystem system,
                           Spectra spectra) throws ExecutionError {
        String jsonRequest = Messages.serialize(DiagnosticMessages.issueRequest(system, spectra));

        try {
            DiagnosticNative libdiag = (DiagnosticNative) Native.loadLibrary("diag", DiagnosticNative.class);

            PointerByReference ptrRef = new PointerByReference();
            libdiag.run(jsonRequest, ptrRef);
            Pointer p = ptrRef.getValue();
            String diagnosticReport = p.getString(0);
            System.out.println("diagnosticReport: " + diagnosticReport);
            libdiag.cleanup(p);
        }
        catch (Throwable e) {e.printStackTrace();}
    }
}
