package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;

import java.util.BitSet;

/**
 * \brief This class negates the output of some matcher.
 */
public final class NegateMatcher
implements SpectrumMatcher {
    private final SpectrumMatcher matcher;

    public NegateMatcher (SpectrumMatcher matcher) {
        this.matcher = matcher;
    }

    @Override
    public BitSet matchProbes (Spectrum spectrum) {
        BitSet ret = matcher.matchProbes(spectrum);


        ret.flip(0, spectrum.getProbeCount());

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectrum spectrum) {
        BitSet ret = matcher.matchTransactions(spectrum);


        ret.flip(0, spectrum.getTransactionCount());

        return ret;
    }

    @Override
    public String toString () {
        return "Not(" + matcher.toString() + ")";
    }
}