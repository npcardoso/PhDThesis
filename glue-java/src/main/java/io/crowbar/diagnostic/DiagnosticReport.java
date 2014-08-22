package io.crowbar.diagnostic;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import flexjson.JSON;


public final class DiagnosticReport {
    private static class DiagnosticPvt extends Diagnostic {
        private final List<Candidate> candidates;
        private final List<Double> scores;

        DiagnosticPvt (List<Candidate> candidates,
                       List<Double> scores) {
            assert (candidates.size() == scores.size());
            this.candidates = Collections.unmodifiableList(candidates);
            this.scores = Collections.unmodifiableList(scores);
        }

        public List<Double> getScores () {
            return scores;
        }

        public List<Candidate> getCandidates () {
            return candidates;
        }

        @Override
        public DiagnosticElement get (int id) {
            return new DiagnosticElement(candidates.get(id),
                                         scores.get(id));
        }

        @Override
        public int size () {
            return scores.size();
        }
    }

    private static final String GEN_RESULTS = "gen_results";
    private static final String RANK_RESULTS = "rank_results";

    private List<List<Candidate> > generatorResults = new ArrayList<List<Candidate> > ();
    private List<List<Double> > rankerResults = new ArrayList<List<Double> > ();


    DiagnosticReport () {}


    /**
     * @brief Returns the diagnostic for a connection.
     * @pre (candidates = dr.getGeneratorResults.get(c.getFrom()) != null
     * @pre (scores = dr.getRankerResults.get(c.getId())) != null
     * @pre candidates.size() == scores.size()
     */
    @JSON(include = false)
    public Diagnostic getDiagnostic (Connection c) {
        return new DiagnosticPvt(generatorResults.get(c.getFrom()),
                                 rankerResults.get(c.getId()));
    }

    @JSON(name = GEN_RESULTS)
    private void setGeneratorResults (List < List < List < Integer >>> results) {
        generatorResults = new ArrayList<List<Candidate> > (results.size());

        for (List<List<Integer> > generator : results) {
            List<Candidate> candidateList = new ArrayList<Candidate> (generator.size());

            for (List<Integer> candidate : generator) {
                candidateList.add(new Candidate(candidate));
            }

            generatorResults.add(candidateList);
        }
    }

    @JSON(name = RANK_RESULTS)
    private void setRankerResults (List<List<Double> > results) {
        rankerResults = results;
    }

    @JSON(name = GEN_RESULTS)
    private List<List<Candidate> > getGeneratorResults () {
        return generatorResults;
    }

    @JSON(name = RANK_RESULTS)
    private List<List<Double> > getRankerResults () {
        return rankerResults;
    }
}