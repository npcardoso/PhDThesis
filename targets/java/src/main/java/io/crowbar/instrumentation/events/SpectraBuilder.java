package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.spectra.EditableSpectra;
import io.crowbar.instrumentation.spectra.Spectra;
import io.crowbar.instrumentation.spectra.HitTransaction;

import java.util.Arrays;
import java.util.List;
import java.util.LinkedList;
import java.util.regex.Pattern;


public class SpectraBuilder extends AbstractEventListener {
    private int transactionId = 0;
    private boolean error = false;
    private final EditableSpectra spectra = new EditableSpectra();

    /**
     * @TODO: Remove this
     */
    private boolean fallbackAccept = true;
    private final List<String> ignorePatterns = new LinkedList<String> ();

    public SpectraBuilder () {}

    public SpectraBuilder (String... ignorePatterns) {
        this.ignorePatterns.addAll(Arrays.asList(ignorePatterns));
    }

    public SpectraBuilder (List<String> ignorePatterns) {
        this.ignorePatterns.addAll(ignorePatterns);
    }

    public final Spectra getSpectra () {
        return spectra;
    }

    @Override
    public final void registerProbe (Probe probe) throws Exception {}

    @Override
    public final void endTransaction (int probeId,
                                      String exception,
                                      boolean[] hitVector) {
        if (exception != null) {
            for (String ignPat : ignorePatterns) {
                if (Pattern.matches(ignPat, exception)) {
                    reset(false);
                    return;
                }
            }
        }

        HitTransaction t = new HitTransaction(hitVector, error ? 1 : 0, 1, exception);
        spectra.setTransaction(transactionId, t);

        reset(true);
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {
        this.error = this.error || (error > 0);
    }

    private void reset (boolean newTransaction) {
        error = false;

        if (newTransaction)
            transactionId++;
    }
}