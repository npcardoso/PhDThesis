package io.crowbar.instrumentation.spectra;


import io.crowbar.diagnosis.spectra.AbstractTransaction;
import io.crowbar.diagnosis.spectra.HitActivity;

import java.util.AbstractList;


public class HitTransaction extends AbstractTransaction<HitActivity, TransactionMetadata> {
    private static class ActivityListAdaptor extends AbstractList<HitActivity> {
        private final boolean[] hitVector;
        ActivityListAdaptor (boolean[] hitVector) {
            this.hitVector = hitVector;
        }

        @Override
        public final HitActivity get (int index) {
            return new HitActivity(hitVector[index]);
        }

        @Override
        public final int size () {
            return hitVector.length;
        }
    }


    private final String exception;

    public HitTransaction (boolean[] hitVector,
                           double error,
                           double confidence,
                           String exception) {
        super(new ActivityListAdaptor(hitVector), error, confidence);
        this.exception = exception;
    }

    public final String getException () {
        return exception;
    }
}