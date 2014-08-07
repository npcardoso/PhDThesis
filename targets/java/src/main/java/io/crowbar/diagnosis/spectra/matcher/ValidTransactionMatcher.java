package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import io.crowbar.diagnosis.spectra.Transaction;

public class ValidTransactionMatcher
implements SpectraMatcher<Activity, Metadata, Metadata> {
    @Override
    public boolean matchComponent (Spectra< ? extends Activity, ? extends Metadata, ? extends Metadata> spectra,
                                   int componentId) {
        return true;
    }

    @Override
    public boolean matchTransaction (Spectra< ? extends Activity, ? extends Metadata, ? extends Metadata> spectra,
                                     int transactionId) {
        Transaction< ? extends Activity, ? extends Metadata> t = spectra.getTransaction(transactionId);

        for (Activity activity : t) {
            if (activity.isActive())
                return true;
        }

        return false;
    }
}