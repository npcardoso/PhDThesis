package io.crowbar.diagnostic;

public final class DiagnosticElement
    implements Comparable<DiagnosticElement> {
    private final Candidate candidate;
    private final double score;

    public DiagnosticElement (Candidate candidate,
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
    public int compareTo (DiagnosticElement e) {
        return (int) ((e.score - score) * 1e6); // compare with 1e6 precision
    }
}
