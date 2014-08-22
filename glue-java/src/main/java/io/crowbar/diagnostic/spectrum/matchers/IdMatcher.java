package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;
import java.util.BitSet;


/**
 * \brief This class matches components/transactions by id.
 */
public final class IdMatcher
implements SpectrumMatcher<Activity, Metadata> {
    private final BitSet transactions = new BitSet();
    private final BitSet components = new BitSet();

    public IdMatcher (BitSet transactions,
                      BitSet components) {
        this.transactions.or(transactions);
        this.components.or(components);
    }

    public IdMatcher (int[] transactions,
                      int[] components) {
        for (int t : transactions) {
            this.transactions.set(t);
        }

        for (int c : components) {
            this.components.set(c);
        }
    }

    @Override
    public BitSet matchComponents (Spectrum< ? extends Activity, ? extends Metadata> spectrum) {
        return components;
    }

    @Override
    public BitSet matchTransactions (Spectrum< ? extends Activity, ? extends Metadata> spectrum) {
        return transactions;
    }
}
