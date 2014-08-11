package io.crowbar.diagnosis;

import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Component;
import io.crowbar.diagnosis.spectra.Metadata;
import io.crowbar.diagnosis.spectra.Spectra;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.NoSuchElementException;

public final class Candidate<A extends Activity,
                             TM extends Metadata,
                             CM extends Metadata>
implements Iterable<Integer> {
    private final List<Integer> elements = new ArrayList();
    private final Spectra<A, TM, CM> spectra;

    Candidate (Spectra<A, TM, CM> spectra,
               int[] elements) {
        this.spectra = spectra;

        for (int e:elements) {
            assert e >= 0 && e < spectra.getNumComponents();
            this.elements.add(e);
        }
    }

    Candidate (Spectra<A, TM, CM> spectra,
               List<Integer> elements) {
        this.spectra = spectra;

        for (int e:elements) {
            assert e >= 0 && e < spectra.getNumComponents();
            this.elements.add(e);
        }
    }

    public int cardinality () {
        return elements.size();
    }

    public Spectra getSpectra () {
        return spectra;
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