package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import java.util.List;
import java.util.LinkedList;
import java.util.BitSet;

/**
 * \brief This class couples several matchers using some logic operator.
 */
public final class CouplerMatcher
implements SpectrumMatcher {
    public interface Coupler {
        void couple (BitSet ret,
                     BitSet operand);
    }

    private static final class And implements Coupler {
        @Override
        public void couple (BitSet ret,
                            BitSet operand) {
            if (ret.size() < operand.size())
                ret.set(ret.size(), operand.size());

            ret.and(operand);
        }

        @Override
        public String toString () {
            return "&&";
        }
    }

    private static final class Or implements Coupler {
        @Override
        public void couple (BitSet ret,
                            BitSet operand) {
            ret.or(operand);
        }

        @Override
        public String toString () {
            return "||";
        }
    }

    public static final Coupler AND = new And();
    public static final Coupler OR = new Or();


    private final Coupler coupler;
    private final List<SpectrumMatcher> matchers =
        new LinkedList<SpectrumMatcher> ();

    /**
     * \brief Constructs a CouplerMatcher.
     * \param coupler The logical operation.
     * \param matcher A list of matchers.
     */
    public CouplerMatcher (Coupler coupler,
                           SpectrumMatcher... matchers) {
        this.coupler = coupler;

        for (SpectrumMatcher m : matchers) {
            this.matchers.add(m);
        }
    }

    @Override
    public BitSet matchProbes (Spectrum spectrum) {
        BitSet ret = new BitSet();


        for (SpectrumMatcher matcher : matchers) {
            coupler.couple(ret, matcher.matchProbes(spectrum));
        }

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectrum spectrum) {
        BitSet ret = new BitSet();


        for (SpectrumMatcher matcher : matchers) {
            coupler.couple(ret, matcher.matchTransactions(spectrum));
        }

        return ret;
    }

    @Override
    public String toString () {
        StringBuilder str = new StringBuilder();
        boolean first = true;


        for (SpectrumMatcher m : matchers) {
            str.append(m.toString());

            if (!first)
                str.append(" " + coupler.toString() + " ");

            first = false;
        }

        return str.toString();
    }
}