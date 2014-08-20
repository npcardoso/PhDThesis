package io.crowbar.diagnosis.algorithms;

public final class SingleFaultGenerator extends Generator {
    private static final Algorithm algorithm;

    static {
        AlgorithmFactory af = new AlgorithmFactory();
        algorithm = af.create("single_fault");
    }


    @Override
    public Algorithm getAlgorithm () {
        return algorithm;
    }
}
