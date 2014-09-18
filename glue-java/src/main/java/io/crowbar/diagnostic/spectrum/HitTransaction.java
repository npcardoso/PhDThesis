package io.crowbar.diagnostic.spectrum;


import java.util.BitSet;


public class HitTransaction
extends Transaction {
    private final BitSet activity;
    private final double error;
    private final double confidence;


    public HitTransaction (int id,
                           boolean[] activity,
                           double error,
                           double confidence) {
        super(id);
        this.activity = new BitSet();

        for (int i = 0; i < activity.length; i++) {
            this.activity.set(i, activity[i]);
        }

        this.error = error;
        this.confidence = confidence;
    }

    public HitTransaction (int id,
                           BitSet activity,
                           double error,
                           double confidence) {
        super(id);
        this.activity = (BitSet) activity.clone();
        this.error = error;
        this.confidence = confidence;
    }

    @Override
    public final boolean isActive (int id) {
        return activity.get(id);
    }

    @Override
    public final double getError () {
        return error;
    }

    @Override
    public final double getConfidence () {
        return confidence;
    }

    @Override
    public final int numActive () {
        return activity.cardinality();
    }

    @Override
    public final int size () {
        return activity.length();
    }
}