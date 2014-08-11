package io.crowbar.diagnosis.algorithms;

import io.crowbar.diagnosis.algorithms.AlgorithmFactory.Algorithm;


import flexjson.JSONSerializer;
import flexjson.JSON;

import java.util.ArrayList;
import java.util.List;

public final class DiagnosticJob {
    private final int one = 1;
    private final List<Algorithm> generators = new ArrayList<Algorithm> ();
    private final List<Algorithm> rankers = new ArrayList<Algorithm> ();

    public int addGenerator (Algorithm g) {
        generators.add(g);
        return generators.size() - 1;
    }

    public int addRanker (Algorithm r) {
        rankers.add(r);
        return rankers.size() - 1;
    }

    public void addConnection (int generatorId,
                               int rankerId) {}

    private List<Algorithm> getGenerators () {
        return generators;
    }

    private List<Algorithm> getRankers () {
        return rankers;
    }

    @Override
    public String toString () {
        return new JSONSerializer().exclude("*.class").deepSerialize(this);
    }

    public static void main (String[] args) {
        DiagnosticJob j = new DiagnosticJob();


        j.addGenerator(SimilarityRanker.create(SimilarityRanker.Type.OCHIAI));
        j.addRanker(SimilarityRanker.create(SimilarityRanker.Type.OCHIAI));

        System.out.println("Job: " + j);
    }
}