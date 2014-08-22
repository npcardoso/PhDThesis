package io.crowbar.diagnostic.spectrum;

import java.util.Iterator;
import java.util.NoSuchElementException;

public abstract class Spectrum<A extends Activity,
                              TM extends Metadata> {

    private abstract class AbstractIterator<T>
        implements Iterator<T> {
        private int id = 0;

        protected abstract T get (int id);
        protected int getId() {
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

    private class TIterable implements Iterable<Transaction<A, TM>> {
        public Iterator<Transaction<A, TM>> iterator (){
            return new AbstractIterator<Transaction<A, TM> > () {
                @Override
                public boolean hasNext () {
                    return getId() < getTransactionCount();
                }
                @Override
                protected Transaction<A,TM> get (int i) {
                    return getTransaction(i);
                }
            };
        }
    }

    private class CIterable implements Iterable<Component> {
        public Iterator<Component> iterator (){
            return new AbstractIterator<Component> (){
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

    Spectrum() {}

    public abstract Tree getTree ();

    public abstract int getTransactionCount ();
    public abstract int getComponentCount ();

    public abstract Transaction<A, TM> getTransaction (int transactionId);
    public abstract Component getComponent (int componentId);

    public final Iterable<Component> byComponent () {
        return new CIterable();
    }

    public final Iterable<Transaction<A, TM> > byTransaction () {
        return new TIterable();
    }

    @Override
    public final String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Spectrum', ");
        str.append("components=[");
        boolean first = true;

        for(Component component : byComponent()){
            if (!first)
                str.append(",");

            str.append(component.toString());
            first = false;
        }

        str.append("], ");
        str.append("transactions=[");

        first = true;

        for(Transaction<A,TM> transaction : byTransaction()){
            if (!first)
                str.append(",");

            str.append(transaction.toString());
            first = false;
        }

        str.append("]}");

        return str.toString();
    }
}
