package io.crowbar.diagnostic.runners;

import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.messages.DiagnosticMessages;
import io.crowbar.messages.Messages;

import com.sun.jna.Library;
import com.sun.jna.Pointer;
import com.sun.jna.Native;
import com.sun.jna.ptr.PointerByReference;

public class JNARunner implements Runner {
    public interface DiagnosticNative extends Library {
        void run (String request, PointerByReference response);
        void cleanup (Pointer response);
    }

    @Override
    public final DiagnosticReport run (DiagnosticSystem system,
                                       Spectrum spectrum) throws ExecutionError {
        String jsonRequest = Messages.serialize(DiagnosticMessages.issueRequest(system, spectrum));
        String jsonResponse = null;

        try {
            DiagnosticNative libdiag = (DiagnosticNative) Native.loadLibrary("diag", DiagnosticNative.class);
            PointerByReference ptrRef = new PointerByReference();
            libdiag.run(jsonRequest, ptrRef);
            Pointer p = ptrRef.getValue();
            jsonResponse = p.getString(0);
            libdiag.cleanup(p);
        } catch (Throwable e) {
            throw new ExecutionError("JNA crashed with: " + e);
        }

        DiagnosticMessages.Response response = (DiagnosticMessages.Response)
            Messages.getResponseDeserializer().deserialize(jsonResponse);

        return response.getReport();
    }
}
