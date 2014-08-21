package io.crowbar.diagnosis.algorithms;

public final class SingleFaultGenerator extends Generator {
    private static final Algorithm ALGORITHM;

    static {
        AlgorithmFactory af = new AlgorithmFactory();
        ALGORITHM = af.create("single_fault");
    }


    @Override
    public Algorithm getAlgorithm () {
        return ALGORITHM;
    }
}
