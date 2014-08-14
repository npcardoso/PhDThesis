package io.crowbar.diagnosis;

import java.util.List;
import java.util.ArrayList;


public class DiagnosticReport {
    public final class Ranking {
        private final class Element implements Comparable<Element> {
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

        private final Connection connection;
        private final List<Element> elements;

        private Ranking (Connection connection,
                         List<Double> scores) {
            elements = new ArrayList<Element> ();

            for (int id = 0; id < scores.size(); id++) {
                elements.add(new Element(id, scores.get(id)));
            }

            this.connection = connection;
        }

        public Connection getConnection () {
            return connection;
        }
    }
}