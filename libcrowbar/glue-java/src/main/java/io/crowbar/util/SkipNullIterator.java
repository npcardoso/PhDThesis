package io.crowbar.util;

import java.util.Iterator;
import java.util.NoSuchElementException;


/**
 * @brief This class iterates over an iterator, skipping null elements
 */
public class SkipNullIterator<T>
implements Iterator<T> {
    private final Iterator<T> it;
    T next;
    private void goToNext () {
        next = null;

        while (it.hasNext()) {
            next = it.next();

            if (next != null)
                break;
        }
    }

    public SkipNullIterator (Iterator<T> it) {
        this.it = it;
        goToNext();
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