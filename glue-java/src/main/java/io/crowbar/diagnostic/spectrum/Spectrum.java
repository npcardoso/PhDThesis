package io.crowbar.diagnostic.spectrum;

import java.util.Iterator;
import java.util.NoSuchElementException;

public abstract class Spectrum<A extends Activity,
                              TM extends Metadata,
                                 CM extends Metadata> {

    private abstract class AbstractIterator<T>
        implements Iterator<T> {
        protected int i = 0;

        protected abstract T get (int i);

        @Override
        public void remove () {
            throw new UnsupportedOperationException();
        }

        @Override
        public T next () {
            if (!hasNext())
                throw new NoSuchElementException();

            return get(i++);
        }

    }

    private class TIterable implements Iterable<Transaction<A, TM>> {
        public Iterator<Transaction<A, TM>> iterator (){
            return new AbstractIterator<Transaction<A, TM> > () {
                @Override
                public boolean hasNext () {
                    return i < getTransactionCount();
                }
                @Override
                protected Transaction<A,TM> get (int i) {
                    return getTransaction(i);
                }
            };
        }
    }

    private class CIterable implements Iterable<Component<CM>> {
        public Iterator<Component<CM>> iterator (){
            return new AbstractIterator<Component<CM>> (){
                @Override
                public boolean hasNext () {
                    return i < getComponentCount();
                }
                @Override
                protected Component<CM> get (int i) {
                    return getComponent(i);
                }
            };
        }
    }

    Spectrum() {}

    public abstract int getTransactionCount ();
    public abstract int getComponentCount ();

    public abstract Transaction<A, TM> getTransaction (int transactionId);
    public abstract Component<CM> getComponent (int componentId);

    public final Iterable<Component<CM> > byComponent () {
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

        for(Component<CM> component : byComponent()){
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
