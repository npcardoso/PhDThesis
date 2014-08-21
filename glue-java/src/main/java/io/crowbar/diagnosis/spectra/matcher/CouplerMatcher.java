package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import java.util.List;
import java.util.LinkedList;
import java.util.BitSet;

/**
 * \brief This class couples several matchers using some logic operator.
 */
public final class CouplerMatcher<A extends Activity,
                                  TM extends Metadata,
                                  CM extends Metadata>
implements SpectraMatcher<A, TM, CM> {
    public interface Coupler {
        void couple (BitSet ret, BitSet operand);
    }

    private static final class And implements Coupler {
        @Override
        public void couple (BitSet ret, BitSet operand) {
            if (ret.size() < operand.size())
                ret.set(ret.size(), operand.size());

            ret.and(operand);
        }
    }

    private static final class Or implements Coupler {
        @Override
        public void couple (BitSet ret, BitSet operand) {
            ret.or(operand);
        }
    }

    private static final Coupler AND = new And();
    private static final Coupler OR = new Or();


    private final Coupler coupler;
    private final List<SpectraMatcher< ? super A, ? super TM, ? super CM> > matchers =
        new LinkedList<SpectraMatcher< ? super A, ? super TM, ? super CM> > ();

    /**
     * \brief Constructs a CouplerMatcher.
     * \param coupler The logical operation.
     * \param matcher A list of matchers.
     */
    public CouplerMatcher (Coupler coupler,
                           SpectraMatcher< ? super A, ? super TM, ? super CM> ... matchers) {
        this.coupler = coupler;

        for (SpectraMatcher< ? super A, ? super TM, ? super CM> m : matchers) {
            this.matchers.add(m);
        }
    }

    @Override
    public BitSet matchComponents (Spectra< ? extends A, ? extends TM, ? extends CM> spectra) {
        BitSet ret = new BitSet();


        for (SpectraMatcher< ? super A, ? super TM, ? super CM> matcher : matchers) {
            coupler.couple(ret, matcher.matchComponents(spectra));
        }

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectra< ? extends A, ? extends TM, ? extends CM> spectra) {
        BitSet ret = new BitSet();


        for (SpectraMatcher< ? super A, ? super TM, ? super CM> matcher : matchers) {
            coupler.couple(ret, matcher.matchTransactions(spectra));
        }

        return ret;
    }
}
