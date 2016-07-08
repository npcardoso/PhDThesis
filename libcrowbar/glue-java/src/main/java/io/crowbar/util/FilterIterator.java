package io.crowbar.util;

import java.util.Iterator;
import java.util.NoSuchElementException;

public final class FilterIterator implements Iterator<Integer> {
    private final Filter filter;
    private int id = 0;

    public FilterIterator (Filter filter) {
        this.filter = filter;
    }

    private boolean ended (int id) {
        return id > filter.getLastFiltered();
    }

    public boolean hasNext () {
        return !ended(filter.next(id));
    }

    public Integer next () {
        int tmp = filter.next(id);


        assert tmp >= 0;

        if (ended(tmp))
            throw new NoSuchElementException();

        id = tmp;
        return id;
    }

    public void remove () {
        throw new UnsupportedOperationException();
    }
}