package io.crowbar.diagnostic;

import java.util.AbstractSet;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;


public final class Candidate
extends AbstractSet<Integer> {
    private final Set<Integer> elements;

    private Candidate () {
        this.elements = new HashSet<Integer> ();
    }

    public Candidate (int... elements) {
        this.elements = new HashSet<Integer> ();

        for (int e:elements) {
            this.elements.add(e);
        }
    }

    public String getCandidateString () {
        return this.getCandidateString('<', '>', ',');
    }

    public String getCandidateString (char tagOpen,
                                      char tagClose,
                                      char delim) {
        StringBuffer cand = new StringBuffer(new String("<"));


        Iterator<Integer> it = this.iterator();

        while (it.hasNext()) {
            Integer c = it.next();
            cand.append(c.toString());
            cand.append(delim);
        }

        cand.setCharAt(cand.length() - 1, tagClose);

        return cand.toString();
    }

    public Candidate (List<Integer> elements) {
        this.elements = new HashSet<Integer> (elements);
    }

    @Override
    public boolean contains (Object o) {
        return elements.contains(o);
    }

    @Override
    public int size () {
        return elements.size();
    }

    @Override
    public Iterator<Integer> iterator () {
        return elements.iterator();
    }

    @Override
    public String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Candidate', ");
        str.append("probes=[");
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