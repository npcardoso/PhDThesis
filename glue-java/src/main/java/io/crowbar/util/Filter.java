package io.crowbar.util;


public interface Filter extends Iterable<Integer> {
    int next (int item);
    int nextFiltered (int item);
    int getLastFiltered ();
    int getFilteredCount ();
    boolean isFiltered (int item);
}