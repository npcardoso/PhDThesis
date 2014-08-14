package io.crowbar.diagnosis.algorithms;

public final class SingleFaultGenerator extends Generator {
    private static class SingleFault extends Algorithm {
        SingleFault () {
            super("single_fault");
        }
    }

    private static final Algorithm algorithm = new SingleFault();

    @Override
    Algorithm getAlgorithm () {
        return algorithm;
    }
}