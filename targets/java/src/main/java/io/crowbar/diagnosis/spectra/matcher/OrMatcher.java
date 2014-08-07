package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import io.crowbar.diagnosis.spectra.Transaction;
import java.util.List;
import java.util.LinkedList;

public class OrMatcher<A extends Activity,
                       TM extends Metadata,
                       CM extends Metadata>
implements SpectraMatcher<A, TM, CM> {
    private List<SpectraMatcher< ? super A, ? super TM, ? super CM> > matchers =
        new LinkedList<SpectraMatcher< ? super A, ? super TM, ? super CM> > ();

    public OrMatcher (SpectraMatcher< ? super A, ? super TM, ? super CM> ... matchers) {
        for (SpectraMatcher< ? super A, ? super TM, ? super CM> m : matchers) {
            this.matchers.add(m);
        }
    }

    @Override
    public boolean matchComponent (Spectra< ? extends A, ? extends TM, ? extends CM> spectra,
                                   int componentId) {
        for (SpectraMatcher< ? super A, ? super TM, ? super CM> matcher : matchers)
            if (matcher.matchComponent(spectra, componentId))
                return true;

        return false;
    }

    @Override
    public boolean matchTransaction (Spectra< ? extends A, ? extends TM, ? extends CM> spectra,
                                     int transactionId) {
        for (SpectraMatcher< ? super A, ? super TM, ? super CM> matcher : matchers)
            if (!matcher.matchTransaction(spectra, transactionId))
                return true;

        return false;
    }
}