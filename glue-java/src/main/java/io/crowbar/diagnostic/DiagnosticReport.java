package io.crowbar.diagnostic;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

import flexjson.JSON;


public final class DiagnosticReport {
    public interface MergeStrategy {
        double reduce(List<Double> scores);
    }

    private static final String GEN_RESULTS="gen_results";
    private static final String RANK_RESULTS="rank_results";

    public static final MergeStrategy AVG =
        new MergeStrategy() {
            @Override
            public double reduce(List<Double> scores) {
                if(scores.size() <= 0) return Double.NaN;
                double total = 0;
                for (Double s : scores) { total += s; }
                return total / scores.size();
            }
        };

    public static final MergeStrategy MAX =
        new MergeStrategy() {
            @Override
            public double reduce(List<Double> scores) {
                if(scores.size() <= 0) return Double.NaN;
                double max = 0;
                for (Double s : scores) { max = Math.max(max, s); }
                return max;
            }
        };

    public static final MergeStrategy SUM =
        new MergeStrategy() {
            @Override
            public double reduce(List<Double> scores) {
                if(scores.size() <= 0) return Double.NaN;
                double total = 0;
                for (Double s : scores) { total += s; }
                return total;
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

    /**
     * @brief Calculates a per component score using an arbitrary merge strategy.
     * This can be used to convert a multiple fault ranking into single fault ranking.
     * @pre (candidates = getGeneratorResults.get(ds.getConnections().get(connectionId).getFrom())) != null
     * @pre (scores = getRankerResults.get(connectionId)) != null
     * @pre candidates.size() == scores.size()
     * @post The size of the output list is equal to the largest component id + 1.
     * @return A list containing the score for each component.
     */
    @JSON(include=false)
    public List<Double> getScorePerNode(DiagnosticSystem ds,
                                        int connectionId,
                                        MergeStrategy ms) {
        List<Candidate> candidates = generatorResults.get(ds.getConnections().get(connectionId).getFrom());
        List<Double> scores = rankerResults.get(connectionId);

        assert(candidates != null);
        assert(scores != null);
        assert(candidates.size() == scores.size());

        ArrayList<List<Double>> tmp = new ArrayList<List<Double>> ();

        for (int i = 0; i < scores.size(); i++) {
            Candidate c = candidates.get(i);
            Double score = scores.get(i);

            for (int cmp : c) {
                tmp.ensureCapacity(cmp + 1);
                while(tmp.size() <= cmp)
                    tmp.add(null);

                List<Double> list = tmp.get(cmp);
                if(list == null) {
                    list = new LinkedList<Double>();
                    tmp.set(cmp, list);
                }

                list.add(score);
            }
        }

        List<Double> ret = new ArrayList(tmp.size());
        for (List<Double> s : tmp) {
            if(s == null)
                ret.add(Double.NaN);
            else
                ret.add(ms.reduce(s));
        }

        return ret;
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
