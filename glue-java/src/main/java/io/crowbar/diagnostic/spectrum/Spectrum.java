package io.crowbar.diagnostic.spectrum;

import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.DiagnosticElement;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.NoSuchElementException;

public abstract class Spectrum<A extends Activity,
                               TM extends Metadata> {
    public interface MergeStrategy {
        double reduce (List<Double> scores);
    }

    public static final MergeStrategy AVG =
        new MergeStrategy() {
        @Override
        public double reduce (List<Double> scores) {
            if (scores.size() <= 0) return Double.NaN;

            double total = 0;

            for (Double s : scores) {total += s;}

            return total / scores.size();
        }
    };

    public static final MergeStrategy MAX =
        new MergeStrategy() {
        @Override
        public double reduce (List<Double> scores) {
            if (scores.size() <= 0) return Double.NaN;

            double max = 0;

            for (Double s : scores) {max = Math.max(max, s);}

            return max;
        }
    };

    public static final MergeStrategy SUM =
        new MergeStrategy() {
        @Override
        public double reduce (List<Double> scores) {
            if (scores.size() <= 0) return Double.NaN;

            double total = 0;

            for (Double s : scores) {total += s;}

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

    private class TIterable implements Iterable<Transaction<A, TM> > {
        public Iterator<Transaction<A, TM> > iterator () {
            return new AbstractIterator<Transaction<A, TM> > () {
                       @Override
                       public boolean hasNext () {
                           return getId() < getTransactionCount();
                       }

                       @Override
                       protected Transaction<A, TM> get (int i) {
                           return getTransaction(i);
                       }
            };
        }
    }

    private class CIterable implements Iterable<Component> {
        public Iterator<Component> iterator () {
            return new AbstractIterator<Component> () {
                       @Override
                       public boolean hasNext () {
                           return getId() < getComponentCount();
                       }

                       @Override
                       protected Component get (int i) {
							return getComponent(i);
                       }
            };
        }
    }

    Spectrum () {}

    public abstract Tree getTree ();

    public abstract int getTransactionCount ();
    public abstract int getComponentCount ();

    public abstract Transaction<A, TM> getTransaction (int transactionId);
    public abstract Component getComponent (int componentId);

	public abstract ArrayList<Component> getComponents() ;
    
    public final Iterable<Component> byComponent () {
        return new CIterable();
    }

    public final Iterable<Transaction<A, TM> > byTransaction () {
        return new TIterable();
    }

    /**
     * @brief Calculates a score value per Tree Node using an arbitrary MergeStrategy.
     * This is used to convert a multiple fault ranking into single fault ranking.
     * @post ret.size() == getTree.size()
     * @return A list containing the score for each node.
     * @throws NoComponentInformationAvailableException 
     */
    public List<Double> getScorePerNode (Diagnostic diagnostic,
                                         MergeStrategy ms) throws NoComponentInformationAvailableException {
        List<List<Double> > tmp = new ArrayList<List<Double> > (getTree().size());

        for (DiagnosticElement e : diagnostic) {
            for (int cmpId : e.getCandidate()) {
                Component cmp = getComponent(cmpId);
                assert (cmp != null);
                int nodeId = cmp.getNode().getId();

                while (tmp.size() <= nodeId)
                    tmp.add(null);

                List<Double> list = tmp.get(cmpId);

                if (list == null) {
                    list = new LinkedList<Double> ();
                    tmp.set(cmpId, list);
                }

                list.add(e.getScore());
            }
        }

        while (tmp.size() <= getTree().size())
            tmp.add(null);

        List<Double> ret = new ArrayList(tmp.size());

        for (List<Double> s : tmp) {
            if (s == null)
                ret.add(Double.NaN);
            else
                ret.add(ms.reduce(s));
        }

        return ret;
    }

    @Override
    public final String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Spectrum', ");
        str.append("components=[");
        boolean first = true;

        for (Component component : byComponent()) {
            if (!first)
                str.append(",");

            str.append(component.toString());
            first = false;
        }

        str.append("], ");
        str.append("transactions=[");

        first = true;

        for (Transaction<A, TM> transaction : byTransaction()) {
            if (!first)
                str.append(",");

            str.append(transaction.toString());
            first = false;
        }

        str.append("]}");

        return str.toString();
    }
}