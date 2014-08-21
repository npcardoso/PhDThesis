package io.crowbar.diagnostic;

import flexjson.JSON;

import java.util.List;
import java.util.ArrayList;

public final class Ranking {
    public final class Element implements Comparable<Element> {
        private final int candidateId;
        private final double score;

        public Element (int candidateId,
                        double score) {
            this.candidateId = candidateId;
            this.score = score;
        }

        public int getCandidateId () {
            return candidateId;
        }

        public double getScore () {
            return score;
        }

        @Override
        public int compareTo (Element o) {
            return (int) ((o.score - score) * 1e6); // compare with 1e6 precision
        }
    }


    private Connection connection = null;
    private List<Element> elements = null;
    private List<Double> scores = null;

    /*! Used for JSON deserialization */
    private Ranking () {}

    /*! Used for JSON deserialization */
    private void setConnection (Connection connection) {
        this.connection = connection;
    }

    /*! Used for JSON deserialization */
    private void setScores (List<Double> scores) {
        elements = new ArrayList<Element> (scores.size());
        scores = new ArrayList<Double> (scores);

        for (int id = 0; id < scores.size(); id++) {
            elements.add(new Element(id, scores.get(id)));
        }
    }

    public Ranking (Connection connection,
                    List<Double> scores) {
        setConnection(connection);
        setScores(scores);
    }

    @JSON
    public Connection getConnection () {
        return connection;
    }
}
