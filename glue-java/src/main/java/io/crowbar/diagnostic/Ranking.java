package io.crowbar.diagnostic;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class Ranking {
    public final class Element implements Comparable<Element> {
        private final Candidate candidate;
        private final double score;

        public Element (Candidate candidate,
                        double score) {
            this.candidate = candidate;
            this.score = score;
        }

        public Candidate getCandidate () {
            return candidate;
        }

        public double getScore () {
            return score;
        }

        @Override
        public int compareTo (Element e) {
            return (int) ((e.score - score) * 1e6); // compare with 1e6 precision
        }
    }

    private List<Element> elements = null;

    /**
     * @brief Creates a ranking.
     * @pre candidates.size () == scores.size()
     */
    public Ranking (List<Candidate> candidates,
                    List<Double> scores) {
        assert(candidates.size() == scores.size());
        elements = new ArrayList<Element> (scores.size());

        for (int id = 0; id < scores.size(); id++) {
            elements.add(new Element(candidates.get(id), scores.get(id)));
        }

        Collections.sort(elements);
    }

    /**
     * @brief Returns the sorted list of ranking elements.
     * @return An unmodifiableList with the sorted elements.
     */
    public List<Element> getElements() {
        return Collections.unmodifiableList(elements);
    }

}
