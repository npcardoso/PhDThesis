package io.crowbar.diagnostic.spectrum;


import java.util.ArrayList;
import java.util.List;


public final class TransactionFactory<A extends Activity,
                                      TM extends Metadata> {
    private static final class Tr<A extends Activity,
                                  TM extends Metadata>
    extends Transaction<A, TM> {
        private final List<A> activity = new ArrayList<A> ();
        private final TM metadata;
        private final int active;
        private final double error;
        private final double confidence;

        Tr (int id,
            List< ? extends A> activity,
            double error,
            double confidence,
            TM metadata) {
            super(id);
            int active = 0;

            for (A a : activity) {
                this.activity.add(a);
                active += a.isActive() ? 1 : 0;
            }

            this.active = active;
            this.error = error;
            this.confidence = confidence;
            this.metadata = metadata;
        }

        @Override
        public A get (int id) {
            if (id >= activity.size())
                return null;

            return activity.get(id);
        }

        @Override
        public double getError () {
            return error;
        }

        @Override
        public double getConfidence () {
            return confidence;
        }

        @Override
        public TM getMetadata () {
            return metadata;
        }

        @Override
        public int numActive () {
            return active;
        }

        @Override
        public int size () {
            return activity.size();
        }
    }

    public Transaction<A, TM> create (int id,
                                      List< ? extends A> activity,
                                      double error,
                                      double confidence,
                                      TM metadata) {
        return new Tr<A, TM> (id, activity, error, confidence, metadata);
    }
}
