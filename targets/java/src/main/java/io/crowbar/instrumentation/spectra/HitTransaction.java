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


    public HitTransaction (boolean[] hitVector,
                           double error,
                           double confidence,
                           TransactionMetadata metadata) {
        super(new ActivityListAdaptor(hitVector), error, confidence, metadata);
    }
}