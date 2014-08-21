package io.crowbar.diagnostic;

import java.lang.reflect.Type;
import java.util.List;
import java.util.ArrayList;

import flexjson.JSON;
import flexjson.ObjectFactory;
import flexjson.ObjectBinder;
import flexjson.JsonNumber;
import flexjson.JSONException;
import java.util.Map;


public final class DiagnosticReport {
    private static final String GEN_RESULTS="gen_results";
    private static final String RANK_RESULTS="rank_results";

    private List<List<Candidate> > generatorResults = new ArrayList<List<Candidate> > ();
    private List<List<Double> > rankerResults = new ArrayList<List<Double> > ();

    DiagnosticReport(){}

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
