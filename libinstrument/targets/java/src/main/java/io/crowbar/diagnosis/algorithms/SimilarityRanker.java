package io.crowbar.diagnosis.algorithms;

import io.crowbar.diagnosis.Algorithm;
import io.crowbar.diagnosis.AlgorithmFactory;

public final class SimilarityRanker extends Ranker {
    public static enum Type {
        OCHIAI("ochiai"),
        JACCARD("jaccard");

        private final String name;
        private final Algorithm algorithm;

        private Type(String name) {
            AlgorithmFactory af = new AlgorithmFactory();


            af.setConfig("type", name);
            algorithm = af.create("similarity");
            this.name = name;
        }

        final Algorithm getAlgorithm () {
            return algorithm;
        }

        final String getName () {
            return name;
        }
    }

    private final Type type;
    public SimilarityRanker (Type type) {
        this.type = type;
    }

    @Override
    public Algorithm getAlgorithm () {
        return type.getAlgorithm();
    }
}