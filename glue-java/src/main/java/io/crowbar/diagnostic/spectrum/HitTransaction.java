package io.crowbar.diagnostic.spectrum;


import java.util.ArrayList;
import java.util.List;
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
    public boolean isActive (int id) {
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
    public int numActive () {
        return activity.cardinality();
    }

    @Override
    public int size () {
        return activity.length();
    }
}