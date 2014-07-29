package io.crowbar.diagnosis;

import java.util.ArrayList;

public class Spectra {
    private final ArrayList<boolean[]> activity = new ArrayList<boolean[]> ();
    private final ArrayList<Boolean> error = new ArrayList<Boolean> ();

    public final void setActivity (int transactionId,
                                   boolean[] hitVector) {
        activity.ensureCapacity(transactionId + 1);

        while (activity.size() <= transactionId) {
            activity.add(null);
        }

        boolean[] hv = hitVector.clone();
        activity.set(transactionId, hv);
    }

    public final void setError (int transactionId,
                                boolean err) {
        error.ensureCapacity(transactionId + 1);

        while (error.size() <= transactionId) {
            error.add(false);
        }

        error.set(transactionId, err);
    }

    public final int getNumComponents () {
        int numComponents = 0;


        for (boolean[] hv : activity) {
            numComponents = Math.max(numComponents, hv.length);
        }

        return numComponents;
    }

    public final int getNumTransactions () {
        return Math.max(activity.size(),
                        error.size());
    }

    public final boolean getActivity (int transactionId,
                                      int componentId) {
        if (transactionId < 0 || transactionId >= activity.size())
            return false;

        boolean[] hv = activity.get(transactionId);

        if (componentId < 0 || componentId >= hv.length)
            return false;

        return hv[componentId];
    }

    public final boolean getError (int transactionId) {
        if (transactionId < 0 || transactionId >= error.size())
            return false;

        Boolean ret = error.get(transactionId);
        System.err.println("getError: " + ret);
        return (ret == null) ? false : ret;
    }

    @Override
    public final String toString () {
        int numComponents = getNumComponents();
        int numTransactions = getNumTransactions();

        StringBuffer ret = new StringBuffer();


        ret.append(numComponents);
        ret.append(" ");
        ret.append(numTransactions);

        for (int i = 0; i < numTransactions; i++) {
            ret.append("\n");

            for (int j = 0; j < numComponents; j++) {
                ret.append(getActivity(i, j) ? "1 " : "0 ");
            }

            ret.append(getError(i) ? "-" : "+");
        }

        return ret.toString();
    }
}