package io.crowbar.instrumentation.spectra;

import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Transaction;
import io.crowbar.diagnosis.spectra.HitSpectraSerializer;

public final class SpectraSerializer {
    public static String serialize (Spectra spectra) {
        int numComp = spectra.getNumComponents();
        int numTran = spectra.getNumTransactions();
        StringBuilder str = new StringBuilder();
        boolean first = true;


        str.append("[");

        for (int i = 0; i < numComp; i++) {
            ComponentMetadata m = spectra.getComponent(i).getMetadata();

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


        str.append(numComp);
        str.append(" ");
        str.append(numTran);
        str.append("\n");

        for (Transaction< ? extends Activity, TransactionMetadata> t : spectra) {
            str.append(HitSpectraSerializer.serialize(t, numComp));
            str.append(" # exceptionClass: " + t.getMetadata().getExceptionClass());
            str.append(", exceptionMessage: " + t.getMetadata().getExceptionMessage());
            str.append("\n");
        }

        return str.toString();
    }
}