package io.crowbar.diagnosis.spectra;

public final class HitSpectraSerializer {
    public static String serialize (Spectra< ? extends Transaction, ? > spectra) {
        int numComp = spectra.getNumComponents();
        int numTran = spectra.getNumTransactions();
        StringBuilder str = new StringBuilder();


        str.append(numComp);
        str.append(" ");
        str.append(numTran);
        str.append("\n");

        for (Transaction t : spectra) {
            for (int i = 0; i < numComp; i++) {
                str.append(t.getCount(i));
                str.append(" ");
            }

            str.append(t.getError());
            str.append("\n");
        }

        return str.toString();
    }
}