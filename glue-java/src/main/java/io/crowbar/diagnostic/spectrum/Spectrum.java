package io.crowbar.diagnostic.spectrum;

import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.DiagnosticElement;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.NoSuchElementException;

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


    private abstract class AbstractIterator<T>
    implements Iterator<T> {
        private int id = 0;

        protected abstract T get (int id);
        protected int getId () {
            return id;
        }

        @Override
        public void remove () {
            throw new UnsupportedOperationException();
        }

        @Override
        public T next () {
            if (!hasNext())
                throw new NoSuchElementException();

            return get(id++);
        }
    }

    private class TIterable implements Iterable<Transaction> {
        public Iterator<Transaction> iterator () {
            return new AbstractIterator<Transaction> () {
                       @Override
                       public boolean hasNext () {
                           return getId() < getTransactionCount();
                       }

                       @Override
                       protected Transaction get (int i) {
                           return getTransaction(i);
                       }
            };
        }
    }

    private class PIterable implements Iterable<Probe> {
        public Iterator<Probe> iterator () {
            return new AbstractIterator<Probe> () {
                       @Override
                       public boolean hasNext () {
                           return getId() < getProbeCount();
                       }

                       @Override
                       protected Probe get (int i) {
                           return getProbe(i);
                       }
            };
        }
    }

    Spectrum () {}

    public abstract Tree getTree ();

    public abstract int getTransactionCount ();
    public abstract int getProbeCount ();

    public abstract Transaction getTransaction (int transactionId);

    public abstract Probe getProbe (int probeId);

    public abstract List<Probe> getProbes ();

    public final List<Probe> getNodeProbes (int nodeId) {
        List<Probe> nodeProbes = new ArrayList<Probe> ();

        for (Probe p : getProbes()) {
            if (p.getNodeId() == nodeId)
                nodeProbes.add(p);
        }

        return nodeProbes;
    }

    public final Iterable<Probe> byProbe () {
        return new PIterable();
    }

    public final Iterable<Transaction> byTransaction () {
        return new TIterable();
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
     * @post ret.size() == getTree.size()
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