package io.crowbar.diagnostic;

import java.util.Collections;
import java.util.List;

public class UnsortedDiagnostic extends Diagnostic {
    private final List<Candidate> candidates;
    private final List<Double> scores;

    public UnsortedDiagnostic (List<Candidate> candidates,
                               List<Double> scores) {
        assert (candidates.size() == scores.size());
        this.candidates = Collections.unmodifiableList(candidates);
        this.scores = Collections.unmodifiableList(scores);
    }

    /*  public List<Double> getScores () {
     *    return scores;
     * }
     *
     * public List<Candidate> getCandidates () {
     *    return candidates;
     * }
     */
    @Override
    public DiagnosticElement get (int id) {
        return new DiagnosticElement(candidates.get(id),
                                     scores.get(id));
    }

    @Override
    public int size () {
        return scores.size();
    }
}