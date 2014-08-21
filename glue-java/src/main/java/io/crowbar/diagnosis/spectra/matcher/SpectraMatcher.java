package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;

import java.util.BitSet;

public interface SpectraMatcher<A extends Activity,
                                TM extends Metadata,
                                CM extends Metadata> {
    /**
     * \brief Returns a BitSet encoding the component matches.
     * \post ret.size() == spectra.getComponentCount()
     * \return a BitSet ret where ret.get(i) represents the matching result for component i.
     */
    BitSet matchComponents (Spectra< ? extends A, ? extends TM, ? extends CM> spectra);
    /**
     * \brief Returns a BitSet encoding the transaction matches.
     * \post ret.size() == spectra.getTransactionCount()
     * \return a BitSet ret where ret.get(i) represents the matching result for transaction i.
     */
    BitSet matchTransactions (Spectra< ? extends A, ? extends TM, ? extends CM> spectra);
}
