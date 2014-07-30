package io.crowbar.instrumentation.spectra;

public final class HitSpectraSerializer {
    public static String serialize (Spectra spectra) {
        int numComp = spectra.getNumComponents();
        StringBuilder str = new StringBuilder();
        boolean first = true;


        str.append("[");

        for (int i = 0; i < numComp; i++) {
            Metadata m = spectra.getMetadata(i);

            if (!first)
                str.append(", ");

            str.append(i + ": (");
            str.append(m.getType().getName());
            str.append(", ");
            str.append(m.getNode().getFullName(":", 1));
            str.append(")");
            first = false;
        }

        str.append("]\n");

        return str.toString() + io.crowbar.diagnosis.spectra.HitSpectraSerializer.serialize(spectra);
    }
}