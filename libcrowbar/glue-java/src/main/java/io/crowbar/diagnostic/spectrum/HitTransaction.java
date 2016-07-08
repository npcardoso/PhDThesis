package io.crowbar.diagnostic.spectrum;


import java.util.BitSet;


public class HitTransaction
extends Transaction {
    private BitSet activity;
    private final double error;
    private final double confidence;


    public HitTransaction (int id,
                           double error,
                           double confidence) {
        super(id);
        this.error = error;
        this.confidence = confidence;
        this.activity = new BitSet();
    }

    public HitTransaction (int id,
                           boolean[] activity,
                           double error,
                           double confidence) {
        this(id, error, confidence);
        this.activity = new BitSet();

        for (int i = 0; i < activity.length; i++) {
            this.activity.set(i, activity[i]);
        }
    }

    public HitTransaction (int id,
                           BitSet activity,
                           double error,
                           double confidence) {
        this(id, error, confidence);
        this.activity = (BitSet) activity.clone();
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