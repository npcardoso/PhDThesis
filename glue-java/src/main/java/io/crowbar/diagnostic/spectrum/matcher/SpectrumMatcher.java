package io.crowbar.diagnostic.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;

import java.util.BitSet;

public interface SpectrumMatcher<A extends Activity,
                                TM extends Metadata,
                                CM extends Metadata> {
    /**
     * \brief Returns a BitSet encoding the component matches.
     * \post ret.size() == spectrum.getComponentCount()
     * \return a BitSet ret where ret.get(i) represents the matching result for component i.
     */
    BitSet matchComponents (Spectrum< ? extends A, ? extends TM, ? extends CM> spectrum);
    /**
     * \brief Returns a BitSet encoding the transaction matches.
     * \post ret.size() == spectrum.getTransactionCount()
     * \return a BitSet ret where ret.get(i) represents the matching result for transaction i.
     */
    BitSet matchTransactions (Spectrum< ? extends A, ? extends TM, ? extends CM> spectrum);
}
