package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import java.util.BitSet;

/**
 * \brief This class provides a base class for implementing
 * SpectraMatcher.
 *
 * The default implementation does not match any
 * component/transaction or, alternativelly, matches all
 * components/transactions. The default behavior can be selected using
 * the protected constructor.
 */
public abstract class AbstractSpectraMatcher<A extends Activity,
                                             TM extends Metadata,
                                             CM extends Metadata>
implements SpectraMatcher<A, TM, CM> {
    private final boolean defaultValueComponents;
    private final boolean defaultValueTransactions;


    /**
     * \brief Using this constructor will result in a matcher that, by
     * default, does not match any component/transaction.
     */
    protected AbstractSpectraMatcher () {
        this(false, false);
    }

    /**
     * \brief
     * \param defaultValueTransactions  the default value for transactions.
     * \param defaultValueComponents the default value for components.
     */
    protected AbstractSpectraMatcher (boolean defaultValueTransactions,
                                      boolean defaultValueComponents) {
        this.defaultValueTransactions = defaultValueTransactions;
        this.defaultValueComponents = defaultValueComponents;
    }

    @Override
    public BitSet matchComponents (Spectra< ? extends A, ? extends TM, ? extends CM> spectra) {
        BitSet ret = new BitSet();


        if (defaultValueComponents) {
            ret.set(0, spectra.getComponentCount());
        }

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectra< ? extends A, ? extends TM, ? extends CM> spectra) {
        BitSet ret = new BitSet();


        if (defaultValueTransactions) {
            ret.set(0, spectra.getTransactionCount());
        }

        return ret;
    }
}
