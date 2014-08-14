package io.crowbar.diagnosis.algorithms;

public final class FuzzinelRanker extends Ranker {
    private static class Fuzzinel extends Algorithm {
        Fuzzinel () {
            super("fuzzinel");
        }
    }

    private final Algorithm algorithm = new Fuzzinel();


    public void useConfidence (boolean val) {
        algorithm.setConfig("confidence", "" + val);
    }

    public void useFuzzyError (boolean val) {
        algorithm.setConfig("fuzzy_error", "" + val);
    }

    @Override
    Algorithm getAlgorithm () {
        return algorithm;
    }
}