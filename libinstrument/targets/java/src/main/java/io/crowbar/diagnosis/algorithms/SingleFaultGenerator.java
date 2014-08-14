package io.crowbar.diagnosis.algorithms;

import io.crowbar.diagnosis.Algorithm;
import io.crowbar.diagnosis.AlgorithmFactory;

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