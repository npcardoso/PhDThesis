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
        if (Double.isNaN(e.score)) {
            if (Double.isNaN(score))
                return 0;
            else return -1;
        } else {
            if (Double.isNaN(score))
                return 1;
        }

        return -Double.compare(score, e.score);
        /*double diff = (e.score - score);
         *
         * if(Double.isNAN(e.score) && Double.isNAN(score))
         *  return 0;
         * if(diff > 0)
         *  return 1;
         * if(diff < 0)
         *  return -1;
         *  return 0;*/
    }
}