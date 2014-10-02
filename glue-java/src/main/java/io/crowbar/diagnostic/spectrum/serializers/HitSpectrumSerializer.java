package io.crowbar.diagnostic.spectrum.serializers;


import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Spectrum;


public final class HitSpectrumSerializer {
    public static String serialize (Spectrum spectrum) {
        return HitSpectrumSerializer.serialize(spectrum, "\n");
    }

    public static String serialize (Spectrum spectrum,
                                    String separator) {
        int numComp = spectrum.getProbeCount();
        int numTran = spectrum.getTransactionCount();
        StringBuilder str = new StringBuilder();


        str.append(numComp);
        str.append(" ");
        str.append(numTran);
        str.append(separator);

        for (Transaction t : spectrum.byTransaction()) {
            serialize(str, t, numComp);
            str.append(separator);
        }

        return str.toString();
    }

    public static void serialize (StringBuilder str,
                                  Transaction transaction,
                                  int probeCount) {
        int i = 0;


        if (transaction != null) {
            for (i = 0; i < transaction.size(); i++) {
                str.append(transaction.isActive(i) ? "1" : "0");
                str.append(" ");
            }
        }

        for (; i < probeCount; i++) {
            str.append(0);
            str.append(" ");
        }

        str.append(transaction == null ? 0 : transaction.getError());
        // str.append(" ");
        // str.append(transaction == null ? 0 : transaction.getConfidence());
    }

    public static String serialize (Transaction transaction,
                                    int probeCount) {
        StringBuilder str = new StringBuilder();


        serialize(str, transaction, probeCount);

        return str.toString();
    }
}