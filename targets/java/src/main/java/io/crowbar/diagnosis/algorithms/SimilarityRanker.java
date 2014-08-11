package io.crowbar.diagnosis.algorithms;

public final class SimilarityRanker {
    public enum Type {
        OCHIAI("ochiai"),
        JACCARD("jaccard");

        private final String name;
        private Type(String name) {
            this.name = name;
        }

        public final String getName () {
            return name;
        }
    }

    private SimilarityRanker () {}

    public static AlgorithmFactory.Algorithm create (Type type) {
        AlgorithmFactory af = new AlgorithmFactory();


        af.addConfig("type", type.getName());
        return af.create("similarity");
    }
}