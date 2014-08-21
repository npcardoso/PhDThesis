package io.crowbar.diagnosis.spectra;

import java.util.Iterator;
import java.util.NoSuchElementException;

public abstract class Spectra<A extends Activity,
                              TM extends Metadata,
                              CM extends Metadata>
implements Iterable<Transaction<A, TM> > {
    private abstract class AbstractIterator<T>
    implements Iterator<T> {
        @Override
        public void remove () {
            throw new UnsupportedOperationException();
        }
    }

    private class TIterator
    extends AbstractIterator<Transaction<A, TM> > {
        private int i = 0;

        @Override
        public boolean hasNext () {
            return i < getNumTransactions();
        }

        @Override
        public Transaction<A, TM> next () {
            if (!hasNext())
                throw new NoSuchElementException();

            return getTransaction(i++);
        }
    }


    private class CIterator
    extends AbstractIterator<Component<CM> > {
        private int i = 0;

        @Override
        public boolean hasNext () {
            return i < getNumComponents();
        }

        @Override
        public Component<CM> next () {
            if (!hasNext())
                throw new NoSuchElementException();

            return getComponent(i++);
        }
    }


    public abstract int getNumTransactions ();
    public abstract int getNumComponents ();

    public abstract Transaction<A, TM> getTransaction (int transactionId);
    public abstract Component<CM> getComponent (int componentId);

    public final Iterator<Component<CM> > componentsIterator () {
        return new CIterator();
    }

    public final Iterator<Transaction<A, TM> > transactionsIterator () {
        return new TIterator();
    }

    public final Iterator<Transaction<A, TM> > iterator () {
        return new TIterator();
    }

    @Override
    public final String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Spectra', ");
        str.append("components=[");
        boolean first = true;
        Iterator it = this.componentsIterator();

        while (it.hasNext()) {
            if (!first)
                str.append(",");

            str.append(it.next().toString());
            first = false;
        }

        str.append("], ");
        str.append("transactions=[");

        first = true;
        it = this.transactionsIterator();

        while (it.hasNext()) {
            if (!first)
                str.append(",");

            str.append(it.next().toString());
            first = false;
        }

        str.append("]}");

        return str.toString();
    }
}
