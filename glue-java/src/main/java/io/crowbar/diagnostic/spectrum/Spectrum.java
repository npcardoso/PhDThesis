package io.crowbar.diagnostic.spectrum;

import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.DiagnosticElement;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.AbstractList;

public abstract class Spectrum {
    public interface MergeStrategy {
        double reduce (List<Double> scores);
    }

    public static final MergeStrategy AVG =
        new MergeStrategy() {
        @Override
        public double reduce (List<Double> scores) {
            if (scores.size() <= 0) return Double.NaN;

            double total = 0;

            for (Double s : scores) {
                total += s;
            }

            return total / scores.size();
        }
    };

    public static final MergeStrategy MAX =
        new MergeStrategy() {
        @Override
        public double reduce (List<Double> scores) {
            if (scores.size() <= 0) return Double.NaN;

            double max = 0;

            for (Double s : scores) {
                max = Math.max(max, s);
            }

            return max;
        }
    };

    public static final MergeStrategy SUM =
        new MergeStrategy() {
        @Override
        public double reduce (List<Double> scores) {
            if (scores.size() <= 0) return Double.NaN;

            double total = 0;

            for (Double s : scores) {
                total += s;
            }

            return total;
        }
    };

    private class SkipNullIterator<T>
    implements Iterator<T> {
        private final Iterator<T> it;
        T next;

        SkipNullIterator (Iterator<T> it) {
            this.it = it;
            goToNext();
        }

        private void goToNext () {
            next = null;

            while (it.hasNext()) {
                next = it.next();

                if (next != null)
                    break;
            }
        }

        @Override
        public final boolean hasNext () {
            return next != null;
        }

        @Override
        public final void remove () {
            throw new UnsupportedOperationException();
        }

        @Override
        public final T next () {
            if (!hasNext())
                throw new NoSuchElementException();

            T tmp = next;

            goToNext();
            return tmp;
        }
    }

    private final List<Transaction> transactions =
        new AbstractList() {
        @Override
        public int size () {
            return getTransactionCount();
        }

        @Override
        public Transaction get (int id) {
            return getTransaction(id);
        }
    };

    private final List<Probe> probes =
        new AbstractList() {
        @Override
        public int size () {
            return getProbeCount();
        }

        @Override
        public Probe get (int id) {
            return getProbe(id);
        }
    };


    Spectrum () {}

    public abstract Tree getTree ();

    public abstract int getTransactionCount ();
    public abstract int getProbeCount ();

    /**
     * @brief Retreives a transaction by id.
     * @return A transaction or null if a transaction with such id
     * does not exist.
     */
    public abstract Transaction getTransaction (int transactionId);

    /**
     * @brief Retreives a probe by id.
     * @return A probe or null if a probe with such id does
     * not exist/is not linked with any node in the tree.
     */
    public abstract Probe getProbe (int probeId);


    /**
     * @brief Returns an immutable list of transactions.
     * @note Uses getTransaction()
     */
    public final List<Transaction> getTransactions () {
        return transactions;
    }

    /**
     * @brief Returns an immutable list of probes.
     */
    public final List<Probe> getProbes () {
        return probes;
    }

    // TODO: Implement List as a view instead of actually creating a list
    public final List<Probe> getNodeProbes (int nodeId) {
        List<Probe> nodeProbes = new ArrayList<Probe> ();

        for (Probe p : getProbes()) {
            if (p.getNodeId() == nodeId)
                nodeProbes.add(p);
        }

        return nodeProbes;
    }

    /**
     * @brief Returns an iterable over the spectrum's transactions. null transactions are ommited.
     * The transactions are iterated in ascending ID order.
     */
    public final Iterable<Transaction> byTransaction () {
        return new Iterable() {
                   @Override
                   public Iterator<Transaction> iterator () {
                       return new SkipNullIterator(getTransactions().iterator());
                   }
        };
    }

    /**
     * @brief Returns an iterable over the spectrum's probes. null probes are ommited.
     * The probes are iterated in ascending ID order.
     */
    public final Iterable<Probe> byProbe () {
        return new Iterable() {
                   @Override
                   public Iterator<Probe> iterator () {
                       return new SkipNullIterator(getProbes().iterator());
                   }
        };
    }

    private List<Double> reduce (List<List<Double> > l,
                                 MergeStrategy ms) {
        List<Double> ret = new ArrayList<Double> (l.size());

        for (List<Double> s : l) {
            if (s == null)
                ret.add(Double.NaN);
            else
                ret.add(ms.reduce(s));
        }

        return ret;
    }

    /**
     * @brief Calculates a score value per Tree Node using an arbitrary MergeStrategy.
     * This is used to convert a multiple fault ranking into single fault ranking.
     * @post ret.size() == getTree().size()
     * @return A list containing the score for each node.
     */
    public final List<Double> getScorePerNode (Diagnostic diagnostic,
                                               MergeStrategy ms) {
        List<List<Double> > tmp = new ArrayList<List<Double> > (getTree().size());

        while (tmp.size() < getTree().size()) {
            tmp.add(null);
        }

        for (DiagnosticElement e : diagnostic) {
            for (int probeId : e.getCandidate()) {
                Probe probe = getProbe(probeId);

                if (probe == null) continue; // Ignore probes without information

                int nodeId = probe.getNode().getId();
                List<Double> list = tmp.get(nodeId);

                if (list == null) {
                    list = new LinkedList<Double> ();
                    tmp.set(nodeId, list);
                }

                list.add(e.getScore());
            }
        }

        return reduce(tmp, ms);
    }

    /**
     * @brief Calculates a score value per Probe using an arbitrary MergeStrategy.
     * This is used to convert a multiple fault ranking into single fault ranking.
     * @post ret.size() == getProbeCount()
     * @return A list containing the score for each probe.
     */
    public final List<Double> getScorePerProbe (Diagnostic diagnostic,
                                                MergeStrategy ms) {
        List<List<Double> > tmp = new ArrayList<List<Double> > (getProbeCount());

        while (tmp.size() < getProbeCount()) {
            tmp.add(null);
        }

        for (DiagnosticElement e : diagnostic) {
            for (int probeId : e.getCandidate()) {
                List<Double> list = tmp.get(probeId);

                if (list == null) {
                    list = new LinkedList<Double> ();
                    tmp.set(probeId, list);
                }

                list.add(e.getScore());
            }
        }

        return reduce(tmp, ms);
    }

    @Override
    public final boolean equals (Object o) {
        if (!(o instanceof Spectrum))
            return false;

        Spectrum s = (Spectrum) o;

        if (s.getTransactionCount() != getTransactionCount())
            return false;

        if (s.getProbeCount() != getProbeCount())
            return false;

        if (!s.getProbes().equals(getProbes()))
            return false;

        if (!s.getTree().equals(getTree()))
            return false;

        Iterator<Transaction> tIt = s.byTransaction().iterator();

        for (Transaction t : byTransaction()) {
            Transaction other = tIt.next();

            if (other == null || t == null) {
                if (t != null || other != null)
                    return false;
            } else if (!t.equals(other))
                return false;
        }

        Iterator<Probe> pIt = s.byProbe().iterator();

        for (Probe p : byProbe()) {
            Probe other = pIt.next();

            if (other == null || p == null) {
                if (p != null || other != null)
                    return false;
            } else if (!p.equals(other))
                return false;
        }

        return true;
    }

    @Override
    public final String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Spectrum', ");
        str.append("probes=[");
        boolean first = true;

        for (Probe probe : byProbe()) {
            if (!first)
                str.append(",");

            if (probe == null)
                str.append("null");
            else
                str.append(probe.toString());

            first = false;
        }

        str.append("], ");
        str.append("transactions=[");

        first = true;

        for (Transaction transaction : byTransaction()) {
            if (!first)
                str.append(",");

            str.append(transaction.toString());
            first = false;
        }

        str.append("]}");

        return str.toString();
    }
}