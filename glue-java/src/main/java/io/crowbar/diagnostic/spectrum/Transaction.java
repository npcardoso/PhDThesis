package io.crowbar.diagnostic.spectrum;

import java.util.Iterator;
import java.util.NoSuchElementException;

public abstract class Transaction
implements Iterable<Integer> {
    private class TransactionIterator implements Iterator<Integer> {
        private int i = 0;

        TransactionIterator () {
            goToNext();
        }

        private void goToNext () {
            while (i < size() && !isActive(i)) {
                i++;
            }
        }

        @Override
        public boolean hasNext () {
            return i < size();
        }

        @Override
        public Integer next () {
            if (!hasNext())
                throw new NoSuchElementException();

            int tmp = i++;
            goToNext();
            return tmp;
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

    /**
     * @brief Checks if probe is active.
     * @note iterator() uses this function.
     * @return the activity or false if probe does not exist.
     */
    public abstract boolean isActive (int id);

    public final boolean isError () {
        return getError() >= 1;
    }

    public abstract double getError ();

    public abstract double getConfidence ();

    public abstract int numActive ();

    /**
     * @brief Returns the original transaction.
     * This can be used when using TransactionViews to get the backing transaction object.
     * @note if t.getOriginal() == t, t is not a view.
     */
    public Transaction getOriginal () {
        return this;
    }

    /**
     * @brief Returns an iterator that iterates over active probes.
     */
    @Override
    public final Iterator<Integer> iterator () {
        return new TransactionIterator();
    }

    /**
     * @brief Gets the size of the transaction
     * @note iterator uses this function.
     * @return The size of the transaction is equal to the id of the
     * last active probe + 1.
     */
    public abstract int size ();

    @Override
    public boolean equals (Object o) {
        if (!(o instanceof Transaction))
            return false;

        Transaction t = (Transaction) o;

        if (getError() != t.getError())
            return false;

        if (getConfidence() != t.getConfidence())
            return false;

        Iterator it = t.iterator();

        for (Integer id : this) {
            if (!id.equals(it.next()))
                return false;
        }

        return true;
    }

    @Override
    public String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Transaction', ");
        str.append("id=" + getId() + ", ");
        str.append("activity=[");
        boolean first = true;

        for (Integer id : this) {
            if (!first)
                str.append(",");

            str.append(id.toString());
            first = false;
        }

        str.append("], ");
        str.append("error=" + getError() + ", ");
        str.append("confidence=" + getConfidence() + "}");

        return str.toString();
    }
}