package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;
import java.util.List;
import java.util.LinkedList;
import java.util.BitSet;

/**
 * \brief This class couples several matchers using some logic operator.
 */
public final class CouplerMatcher<A extends Activity,
                                  TM extends Metadata,
                                  CM extends Metadata>
implements SpectrumMatcher<A, TM> {
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
    private final List<SpectrumMatcher< ? super A, ? super TM> > matchers =
        new LinkedList<SpectrumMatcher< ? super A, ? super TM> > ();

    /**
     * \brief Constructs a CouplerMatcher.
     * \param coupler The logical operation.
     * \param matcher A list of matchers.
     */
    public CouplerMatcher (Coupler coupler,
                           SpectrumMatcher< ? super A, ? super TM> ... matchers) {
        this.coupler = coupler;

        for (SpectrumMatcher< ? super A, ? super TM> m : matchers) {
            this.matchers.add(m);
        }
    }

    @Override
    public BitSet matchComponents (Spectrum< ? extends A, ? extends TM> spectrum) {
        BitSet ret = new BitSet();


        for (SpectrumMatcher< ? super A, ? super TM> matcher : matchers) {
            coupler.couple(ret, matcher.matchComponents(spectrum));
        }

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectrum< ? extends A, ? extends TM> spectrum) {
        BitSet ret = new BitSet();


        for (SpectrumMatcher< ? super A, ? super TM> matcher : matchers) {
            coupler.couple(ret, matcher.matchTransactions(spectrum));
        }

        return ret;
    }
}
