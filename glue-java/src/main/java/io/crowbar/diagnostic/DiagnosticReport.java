package io.crowbar.diagnostic;

import java.util.List;
import java.util.ArrayList;

import flexjson.JSON;


public final class DiagnosticReport {
    public interface MergeStrategy {
        double reduce(List<Double> scores);
    }

    private static final String GEN_RESULTS="gen_results";
    private static final String RANK_RESULTS="rank_results";

    private static final MergeStrategy AVG =
        new MergeStrategy() {
            @Override
            public double reduce(List<Double> scores) {
                if(scores.size() <= 0) return Double.NaN;
                double total = 0;
                for (Double s : scores) { total += s; }
                return total / scores.size();
            }
        };

    private static final MergeStrategy MAX =
        new MergeStrategy() {
            @Override
            public double reduce(List<Double> scores) {
                if(scores.size() <= 0) return Double.NaN;
                double max = 0;
                for (Double s : scores) { max = Math.max(max, s); }
                return max;
            }
        };


    private List<List<Candidate> > generatorResults = new ArrayList<List<Candidate> > ();
    private List<List<Double> > rankerResults = new ArrayList<List<Double> > ();

    DiagnosticReport(){}

    @JSON(include=false)
    public Ranking getRanking (Connection c) {
        return new Ranking(generatorResults.get(c.getFrom()),
                           rankerResults.get(c.getTo()));
    }

    @JSON(include=false)
    public List<Double> getScorePerNode(Connection c,
                                        MergeStrategy ms) {

        return null;
    }


    @JSON(name=GEN_RESULTS)
    private void setGeneratorResults (List<List<List<Integer>>> results) {
        generatorResults = new ArrayList<List<Candidate>>(results.size());
        for(List<List<Integer>> generator : results) {
            List<Candidate> candidateList = new ArrayList<Candidate>(generator.size());
            for(List<Integer> candidate : generator) {
                candidateList.add(new Candidate(candidate));
            }
            generatorResults.add(candidateList);
        }
    }

    @JSON(name=RANK_RESULTS)
    private void setRankerResults(List<List<Double>> results) {
        rankerResults = results;
    }

    @JSON(name=GEN_RESULTS)
    private List<List<Candidate>> getGeneratorResults () {
        return generatorResults;
    }

    @JSON(name=RANK_RESULTS)
    private List<List<Double>> getRankerResults () {
        return rankerResults;
    }
}
