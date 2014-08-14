package io.crowbar.diagnosis.spectra;

import java.util.Iterator;
import java.util.NoSuchElementException;

public abstract class Transaction<A extends Activity,
                                  TM extends Metadata>
implements Iterable<A> {
    private class TransactionIterator implements Iterator<A> {
        private int i = 0;

        @Override
        public boolean hasNext () {
            return i < size();
        }

        @Override
        public A next () {
            if (!hasNext())
                throw new NoSuchElementException();

            return get(i++);
        }

        @Override
        public void remove () {
            throw new UnsupportedOperationException();
        }
    }


    private final int id;

    Transaction (int id) {
        this.id = id;
    }

    public final int getId () {
        return id;
    }

    /*!
     * \brief Returns the activity of component "id".
     * Note: "iterator" uses this function.
     * Should *not* raise any exceptions if the activity does not exist but instead return null.
     */
    public abstract A get (int id);

    public final boolean isError () {
        return getError() >= 1;
    }

    public abstract double getError ();

    public abstract double getConfidence ();

    public abstract TM getMetadata ();

    @Override
    public final Iterator<A> iterator () {
        return new TransactionIterator();
    }

    public abstract int numActive ();


    // ! Note: "iterator" uses this function.
    public abstract int size ();

    @Override
    public String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Transaction', ");
        str.append("id=" + getId() + ", ");
        str.append("activity=[");
        boolean first = true;

        for (A a : this) {
            if (!first)
                str.append(",");

            str.append(a.toString());
            first = false;
        }

        str.append("], ");
        str.append("error=" + getError() + ", ");
        str.append("confidence=" + getConfidence() + ", ");
        str.append("metadata=" + getMetadata() + "}");

        return str.toString();
    }
}