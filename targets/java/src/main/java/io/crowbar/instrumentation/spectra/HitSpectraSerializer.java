package io.crowbar.instrumentation.spectra;

import io.crowbar.instrumentation.runtime.Node;

public final class HitSpectraSerializer {
    public static String serialize (Spectra spectra) {
        int numComp = spectra.getNumComponents();
        StringBuilder str = new StringBuilder();
        boolean first = true;


        str.append("[");

        for (int i = 0; i < numComp; i++) {
            Node n = spectra.getMetadata(i);

            if (!first)
                str.append(", ");

            str.append(i + ": ");
            str.append(n.getFullName(":", 1));
            first = false;
        }

        str.append("]\n");

        return str.toString() + io.crowbar.diagnosis.spectra.HitSpectraSerializer.serialize(spectra);
    }
}