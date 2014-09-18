package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import java.util.BitSet;

/**
 * \brief This class provides a base class for implementing
 * SpectrumMatcher.
 *
 * The default implementation does not match any
 * probe/transaction or, alternativelly, matches all
 * probes/transactions. The default behavior can be selected using
 * the protected constructor.
 */
public abstract class AbstractSpectrumMatcher
implements SpectrumMatcher {
    private final boolean defaultValueProbes;
    private final boolean defaultValueTransactions;


    /**
     * \brief Using this constructor will result in a matcher that, by
     * default, does not match any probe/transaction.
     */
    protected AbstractSpectrumMatcher () {
        this(false, false);
    }

    /**
     * \brief
     * \param defaultValueTransactions  the default value for transactions.
     * \param defaultValueProbes the default value for probes.
     */
    protected AbstractSpectrumMatcher (boolean defaultValueTransactions,
                                       boolean defaultValueProbes) {
        this.defaultValueTransactions = defaultValueTransactions;
        this.defaultValueProbes = defaultValueProbes;
    }

    @Override
    public BitSet matchProbes (Spectrum spectrum) {
        BitSet ret = new BitSet();


        if (defaultValueProbes)
            ret.set(0, spectrum.getProbeCount());


        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectrum spectrum) {
        BitSet ret = new BitSet();


        if (defaultValueTransactions)
            ret.set(0, spectrum.getTransactionCount());


        return ret;
    }
}