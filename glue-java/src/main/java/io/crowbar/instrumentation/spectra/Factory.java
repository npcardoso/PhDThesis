package io.crowbar.instrumentation.spectra;

import io.crowbar.diagnosis.spectra.Component;
import io.crowbar.diagnosis.spectra.ComponentFactory;
import io.crowbar.diagnosis.spectra.Transaction;
import io.crowbar.diagnosis.spectra.TransactionFactory;
import io.crowbar.diagnosis.spectra.activity.Hit;

import java.util.List;
import java.util.AbstractList;


public final class Factory {
    private Factory () {}

    private static class ActivityListAdaptor extends AbstractList<Hit> {
        private final boolean[] hitVector;
        ActivityListAdaptor (boolean[] hitVector) {
            this.hitVector = hitVector;
        }

        @Override
        public final Hit get (int index) {
            return new Hit(hitVector[index]);
        }

        @Override
        public final int size () {
            return hitVector.length;
        }
    }


    public static Transaction<Hit, TrM> createTransaction (int id,
                                                           boolean[] hitVector,
                                                           double error,
                                                           double confidence,
                                                           TrM metadata) {
        List<Hit> adaptor = new ActivityListAdaptor(hitVector);

        TransactionFactory<Hit, TrM> tf =
            new TransactionFactory<Hit, TrM> ();


        return tf.create(id, adaptor, error, confidence, metadata);
    }

    public static Component<CmpM> createComponent (int id,
                                                   CmpM metadata) {
        ComponentFactory<CmpM> cf =
            new ComponentFactory<CmpM> ();
        return cf.create(id, metadata);
    }
}