package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;

import java.util.BitSet;

public interface SpectrumMatcher {
    /**
     * \brief Returns a BitSet encoding the probe matches.
     * \post ret.size() == spectrum.getProbeCount()
     * \return a BitSet ret where ret.get(i) represents the matching result for probe i.
     */
    BitSet matchProbes (Spectrum spectrum);
    /**
     * \brief Returns a BitSet encoding the transaction matches.
     * \post ret.size() == spectrum.getTransactionCount()
     * \return a BitSet ret where ret.get(i) represents the matching result for transaction i.
     */
    BitSet matchTransactions (Spectrum spectrum);
}