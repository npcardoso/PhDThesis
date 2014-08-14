package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import java.util.BitSet;


public final class IdMatcher
implements SpectraMatcher<Activity, Metadata, Metadata> {
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
    public BitSet matchComponents (Spectra< ? extends Activity, ? extends Metadata, ? extends Metadata> spectra) {
        return components;
    }

    @Override
    public BitSet matchTransactions (Spectra< ? extends Activity, ? extends Metadata, ? extends Metadata> spectra) {
        return transactions;
    }
}