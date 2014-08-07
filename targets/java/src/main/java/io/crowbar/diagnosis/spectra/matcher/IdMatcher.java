package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import io.crowbar.diagnosis.spectra.Transaction;

import java.util.HashSet;
import java.util.Set;

public class IdMatcher
implements SpectraMatcher<Activity, Metadata, Metadata> {
    private final Set<Integer> transactions = new HashSet<Integer> ();
    private final Set<Integer> components = new HashSet<Integer> ();

    public IdMatcher (int[] transactions, int[] components) {
        for (int t : transactions)
            this.transactions.add(t);

        for (int c : components)
            this.components.add(c);

        System.out.println("transactions: " + this.components);
        System.out.println("components: " + this.components);
    }

    @Override
    public boolean matchComponent (Spectra< ? extends Activity, ? extends Metadata, ? extends Metadata> spectra,
                                   int componentId) {
        System.out.println("matchComponent: " + componentId + ": " + components.contains(componentId));
        return components.contains(componentId);
    }

    @Override
    public boolean matchTransaction (Spectra< ? extends Activity, ? extends Metadata, ? extends Metadata> spectra,
                                     int transactionId) {
        System.out.println("matchTransaction: " + transactionId + ": " + transactions.contains(transactionId));
        return transactions.contains(transactionId);
    }
}