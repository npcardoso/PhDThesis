package io.crowbar.diagnosis;

import java.util.ArrayList;

public class Spectra {
    private ArrayList<boolean[]> activity = new ArrayList<boolean[]> ();
    private ArrayList<Boolean> error = new ArrayList<Boolean> ();

    public void setActivity (int transactionId,
                             boolean[] hitVector) {
        activity.ensureCapacity(transactionId + 1);

        while (activity.size() <= transactionId) {
            activity.add(null);
        }

        boolean hv[] = new boolean[hitVector.length];
        System.arraycopy(hitVector, 0,
                         hv, 0,
                         hitVector.length);

        activity.set(transactionId, hv);
    }

    public void setError (int transactionId,
                          boolean error) {
        this.error.ensureCapacity(transactionId + 1);

        while (this.error.size() <= transactionId) {
            this.error.add(null);
        }

        this.error.set(transactionId, error);
    }

    public int getNumComponents () {
        int numComponents = 0;


        for (boolean[] hv : activity)
            numComponents = Math.max(numComponents, hv.length);

        return numComponents;
    }

    public int getNumTransactions () {
        return Math.max(activity.size(),
                        error.size());
    }

    public boolean getActivity (int transactionId,
                                int componentId) {
        if (transactionId < 0 || transactionId >= activity.size())
            return false;

        boolean[] hv = activity.get(transactionId);

        if (componentId < 0 || componentId > hv.length)
            return false;

        return hv[componentId];
    }

    public boolean getError (int transactionId) {
        if (transactionId < 0 || transactionId >= error.size())
            return false;

        return error.get(transactionId);
    }

    @Override
    public String toString () {
        int numComponents = getNumComponents();
        int numTransactions = getNumTransactions();

        StringBuffer ret = new StringBuffer();


        ret.append(numComponents);
        ret.append(" ");
        ret.append(numTransactions);

        for (int i = 0; i < numTransactions; i++) {
            ret.append("\n");

            for (int j = 0; j < numComponents; j++)
                ret.append(getActivity(i, j) ? "1 " : "0 ");

            ret.append(getError(i) ? "-" : "+");
        }

        return ret.toString();
    }
}