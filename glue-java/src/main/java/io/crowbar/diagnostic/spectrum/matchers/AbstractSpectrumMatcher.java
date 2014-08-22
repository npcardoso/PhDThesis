package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;
import java.util.BitSet;

/**
 * \brief This class provides a base class for implementing
 * SpectrumMatcher.
 *
 * The default implementation does not match any
 * component/transaction or, alternativelly, matches all
 * components/transactions. The default behavior can be selected using
 * the protected constructor.
 */
public abstract class AbstractSpectrumMatcher<A extends Activity,
                                             TM extends Metadata>
implements SpectrumMatcher<A, TM> {
    private final boolean defaultValueComponents;
    private final boolean defaultValueTransactions;


    /**
     * \brief Using this constructor will result in a matcher that, by
     * default, does not match any component/transaction.
     */
    protected AbstractSpectrumMatcher () {
        this(false, false);
    }

    /**
     * \brief
     * \param defaultValueTransactions  the default value for transactions.
     * \param defaultValueComponents the default value for components.
     */
    protected AbstractSpectrumMatcher (boolean defaultValueTransactions,
                                      boolean defaultValueComponents) {
        this.defaultValueTransactions = defaultValueTransactions;
        this.defaultValueComponents = defaultValueComponents;
    }

    @Override
    public BitSet matchComponents (Spectrum< ? extends A, ? extends TM> spectrum) {
        BitSet ret = new BitSet();


        if (defaultValueComponents) {
            ret.set(0, spectrum.getComponentCount());
        }

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectrum< ? extends A, ? extends TM> spectrum) {
        BitSet ret = new BitSet();


        if (defaultValueTransactions) {
            ret.set(0, spectrum.getTransactionCount());
        }

        return ret;
    }
}
