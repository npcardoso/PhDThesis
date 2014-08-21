package io.crowbar.diagnosis.algorithms;

public final class MHSGenerator extends Generator {
    private final AlgorithmFactory af = new AlgorithmFactory();


    /**
     * \brief Sets the maximum depth of exploration.
     * \pre val >= 1
     */
    public void setMaxDepth (int val) {
        assert (val >= 1);
        af.setConfig("max_depth", "" + val);
    }

    /**
     * \brief Sets the maximum number of computed candidates.
     * \pre val >= 1
     */
    public void setMaxCandidates (int val) {
        assert (val >= 1);
        af.setConfig("max_candidates", "" + val);
    }

    /**
     * \brief Sets the maximum execution time in microseconds.
     * \pre val >= 1
     */
    public void setMaxTime (int val) {
        assert (val >= 1);
        af.setConfig("max_time", "" + val);
    }

    /**
     * \brief Sets the percentage of exploration for each branch.
     * \pre 0 <= val <= 1
     */
    public void setLambda (float val) {
         assert (val >= 0 && val <= 1);
         af.setConfig("lambda", "" + val);
     }

    /**
     * \brief Sets the minimum heuristic score cutoff.
     * \pre 0 <= val <= 1
     */
    public void setMinScore (float val) {
         assert (val >= 0 && val <= 1);
         af.setConfig("lambda", "" + val);
     }

    /**
     * \brief Sets the heuristic type.
     */
    public void setHeuristic (SimilarityRanker.Type type) {
         af.setConfig("heuristic", type.getName());
     }

    @Override
    public Algorithm getAlgorithm () {
        return af.create("mhs2");
    }
}
