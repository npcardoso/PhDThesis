package io.crowbar.diagnosis.spectra;

public class HitTransaction implements Transaction {
    private final boolean[] hitVector;
    private final double error;
    private final double confidence;


    /*!
     * Note: hitVector is cloned
     */
    public HitTransaction (boolean[] hitVector,
                           double error,
                           double confidence) {
        this.hitVector = hitVector.clone();
        this.error = error;
        this.confidence = confidence;
    }

    @Override
    public final int getCount (int componentId) {
        if (componentId < 0 || componentId >= hitVector.length)
            return 0;

        return hitVector[componentId] ? 1 : 0;
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
    public final int size () {
        return hitVector.length;
    }
}