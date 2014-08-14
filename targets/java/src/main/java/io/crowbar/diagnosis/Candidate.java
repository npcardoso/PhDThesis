package io.crowbar.diagnosis;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.NoSuchElementException;

public final class Candidate implements Iterable<Integer> {
    private final List<Integer> elements;

    Candidate (int[] elements) {
        this.elements = new ArrayList();

        for (int e:elements) {
            this.elements.add(e);
        }
    }

    Candidate (List<Integer> elements) {
        this.elements = new ArrayList(elements);
    }

    public int cardinality () {
        return elements.size();
    }

    public final boolean isSingleFault () {
        return cardinality() == 1;
    }

    @Override
    public Iterator<Integer> iterator () {
        return elements.iterator();
    }

    @Override
    public final String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Candidate', ");
        str.append("components=[");
        boolean first = true;

        for (int c : this) {
            if (!first)
                str.append(", ");

            str.append(c);
            first = false;
        }

        str.append("]}");

        return str.toString();
    }
}