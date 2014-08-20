package io.crowbar.diagnosis.algorithms;

public final class MHSGenerator extends Generator {
    private final AlgorithmFactory af = new AlgorithmFactory();


    // ! Maximum depth of exploration ([1;+inf[)
    public void setMaxDepth (int val) {
        assert (val >= 1);
        af.setConfig("max_depth", "" + val);
    }

    // ! Maximum number of computed candidates ([1;+inf[)
    public void setMaxCandidates (int val) {
        assert (val >= 1);
        af.setConfig("max_candidates", "" + val);
    }

    // ! Maximum execution time in microseconds ([1;+inf[)
    public void setMaxTime (int val) {
        assert (val >= 1);
        af.setConfig("max_time", "" + val);
    }

    // ! Percentage of exploration for each branch ([0;1])
    public void setLambda (float val) {
        assert (val >= 0 && val <= 1);
        af.setConfig("lambda", "" + val);
    }

    // ! Minimum heuristic score cutoff ([0;1])
    public void setMinScore (float val) {
        assert (val >= 0 && val <= 1);
        af.setConfig("lambda", "" + val);
    }

    // ! Minimum heuristic score cutoff ([0;1])
    public void setHeuristic (SimilarityRanker.Type type) {
        af.setConfig("heuristic", type.getName());
    }

    @Override
    public Algorithm getAlgorithm () {
        return af.create("mhs2");
    }
}
