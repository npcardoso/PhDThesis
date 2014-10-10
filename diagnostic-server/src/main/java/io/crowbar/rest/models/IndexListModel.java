package io.crowbar.rest.models;

import java.util.AbstractList;

public class IndexListModel extends AbstractList<Integer> {
    private final int min;
    private final int max;

    public IndexListModel (int min,
                           int max) {
        this.min = min;
        this.max = max;
    }

    public IndexListModel (int max) {
        this.min = 0;
        this.max = max;
    }

    @Override
    public int size () {
        return max - min;
    }

    @Override
    public Integer get (int i) {
        return min + i;
    }
}