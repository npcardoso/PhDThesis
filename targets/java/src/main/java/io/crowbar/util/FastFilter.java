package io.crowbar.util;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;


public final class FastFilter implements EditableFilter {
    private final ArrayList<Integer> ff = new ArrayList<Integer> ();
    private int filteredCount = 0;

    public FastFilter () {}

    public FastFilter (Filter f) {
        for (int i = f.nextFiltered(-1); i >= 0; i = f.nextFiltered(i)) {
            filter(i);
        }
    }

    @Override
    public boolean isFiltered (int item) {
        assert item >= 0;

        if (item >= ff.size())
            return false;

        return ff.get(item) != item;
    }

    @Override
    public int next (int item) {
        if (item < 0)
            item = 0;

        if (item >= ff.size())
            return item + 1;
        else
            return ff.get(item + 1);
    }

    @Override
    public int nextFiltered (int item) {
        for (int i = item + 1; i < ff.size(); i++) {
            if (isFiltered(i))
                return i;
        }

        return -1;
    }

    @Override
    public int getLastFiltered () {
        return ff.size() - 1;
    }

    @Override
    public int getFilteredCount () {
        return filteredCount;
    }

    @Override
    public Iterator<Integer> iterator () {
        return new FilterIterator(this);
    }

    @Override
    public void filter (int item) {
        assert item >= 0;

        if (isFiltered(item))
            return;


        ff.ensureCapacity(item + 1);

        while (!(ff.size() > item)) {
            ff.add(ff.size());
        }

        int next = (ff.size() > item + 1) ? ff.get(item + 1) : (item + 1);
        int i = item;

        while (i >= 0 && ff.get(i) >= item) {
            ff.set(i--, next);
        }

        assert isFiltered(item);
        assert isFiltered(ff.size() - 1);

        filteredCount++;
    }

    @Override
    public void filterAll (List<Integer> list) {
        for (int i : list) {
            filter(i);
        }
    }

    @Override
    public boolean equals (Object o) {
        if (!(o instanceof Filter))
            return false;

        Filter f = (Filter) o;

        if (f.getLastFiltered() != getLastFiltered())
            return false;

        for (int i = 0; i < ff.size(); i++) {
            if (f.isFiltered(i) != isFiltered(i))
                return false;
        }

        return true;
    }

    @Override
    public String toString () {
        StringBuilder str = new StringBuilder();


        str.append(getFilteredCount() + ": [");

        for (int i = nextFiltered(-1); i >= 0; i = nextFiltered(i)) {
            str.append(i + ", ");
        }

        str.append("-1]");
        return str.toString();
    }
}