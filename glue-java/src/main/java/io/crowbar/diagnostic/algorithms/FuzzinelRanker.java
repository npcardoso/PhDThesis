package io.crowbar.diagnostic.algorithms;

public final class FuzzinelRanker extends Ranker {
    private final AlgorithmFactory af = new AlgorithmFactory();


    public void useConfidence (boolean val) {
        af.setConfig("confidence", "" + val);
    }

    public void useFuzzyError (boolean val) {
        af.setConfig("fuzzy_error", "" + val);
    }

    @Override
    public Algorithm getAlgorithm () {
        return af.create("fuzzinel");
    }
}
