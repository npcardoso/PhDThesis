package io.crowbar.diagnostic.spectrum.serializers;


import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Spectrum;


public final class HitSpectrumSerializer {
    public static String serialize (Spectrum< ? extends Activity, ?> spectrum) {
        return HitSpectrumSerializer.serialize(spectrum, "\n");
    }

    public static String serialize (Spectrum< ? extends Activity, ?> spectrum,
                                    String separator) {
        int numComp = spectrum.getComponentCount();
        int numTran = spectrum.getTransactionCount();
        StringBuilder str = new StringBuilder();


        str.append(numComp);
        str.append(" ");
        str.append(numTran);
        str.append(separator);

        for (Transaction t : spectrum.byTransaction()) {
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
