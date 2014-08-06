package io.crowbar.diagnosis.spectra;

public class HitSpectraSerializer {
    public static String serialize (Spectra< ? extends Activity, ? > spectra) {
        int numComp = spectra.getNumComponents();
        int numTran = spectra.getNumTransactions();
        StringBuilder str = new StringBuilder();


        str.append(numComp);
        str.append(" ");
        str.append(numTran);
        str.append("\n");

        for (Transaction t : spectra) {
            System.out.println(t);
            int i = 0;

            for (i = 0; i < t.size(); i++) {
                str.append(t.get(i).isActive() ? "1" : "0");
                str.append(" ");
            }

            for (; i < numComp; i++) {
                str.append(0);
                str.append(" ");
            }

            str.append(t.getError());
            str.append("\n");
        }

        return str.toString();
    }
}