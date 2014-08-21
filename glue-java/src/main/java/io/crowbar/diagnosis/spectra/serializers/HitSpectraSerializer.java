package io.crowbar.diagnosis.spectra.serializers;


import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Transaction;
import io.crowbar.diagnosis.spectra.Spectra;


public final class HitSpectraSerializer {
    public static String serialize (Spectra< ? extends Activity, ? , ? > spectra) {
        return HitSpectraSerializer.serialize(spectra, "\n");
    }

    public static String serialize (Spectra< ? extends Activity, ? , ? > spectra,
                                    String separator) {
        int numComp = spectra.getComponentCount();
        int numTran = spectra.getTransactionCount();
        StringBuilder str = new StringBuilder();


        str.append(numComp);
        str.append(" ");
        str.append(numTran);
        str.append(separator);

        for (Transaction t : spectra.byTransaction()) {
            str.append(serialize(t, numComp));
            str.append(separator);
        }

        return str.toString();
    }

    public static String serialize (Transaction< ? extends Activity, ? > transaction,
                                    int componentCount) {
        StringBuilder str = new StringBuilder();
        int i = 0;


        if (transaction != null) {
            for (i = 0; i < transaction.size(); i++) {
                str.append(transaction.get(i).isActive() ? "1" : "0");
                str.append(" ");
            }
        }

        for (; i < componentCount; i++) {
            str.append(0);
            str.append(" ");
        }

        str.append(transaction == null ? 0 : transaction.getError());
//        str.append(" ");
//        str.append(transaction == null ? 0 : transaction.getConfidence());

        return str.toString();
    }
}
