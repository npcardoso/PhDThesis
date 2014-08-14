package io.crowbar.diagnosis.algorithms;

public final class SimilarityRanker extends Ranker {
    private static class Similarity extends Algorithm {
        Similarity (String name) {
            super(name);
        }
    }

    public static enum Type {
        OCHIAI("ochiai"),
        JACCARD("jaccard");

        private final Algorithm algorithm = new Similarity("similarity");
        private Type(String name) {
            algorithm.setConfig("type", name);
        }

        final Algorithm getAlgorithm () {
            return algorithm;
        }
    }

    private final Type type;
    public SimilarityRanker (Type type) {
        this.type = type;
    }

    @Override
    Algorithm getAlgorithm () {
        return type.getAlgorithm();
    }
}