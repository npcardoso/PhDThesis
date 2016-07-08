package io.crowbar.messages;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Tree;

import flexjson.JSON;
import java.util.ArrayList;
import java.util.List;

public class VisualizationMessages {
    private static final String NAME = "visualization";

    public static final class Request
        extends io.crowbar.messages.Request {
        private Tree tree = null;
        private List<Double> scores = null;

        private List<int[]> freqs = null;

        /*! Used for JSON deserialization */
        private Request () {}

        /*! Used for JSON deserialization */
        private void setTree (Tree tree) {
            this.tree = tree;
        }

        /*! Used for JSON deserialization */
        private void setScores (List<Double> scores) {
            this.scores = new ArrayList<Double> ();

            for (Double d : scores) {
                if (Double.isNaN(d))
                    this.scores.add(-1d);
                else
                    this.scores.add(d);
            }
        }

        private void setFreqs (List<int[]> freqs) {
            this.freqs = freqs;
        }

        /**
         * @brief Creates a visualization request.
         * @pre tree.size() == scores.size()
         */
        Request (Tree tree,
                 List<Double> scores,
                 List<int[]> freqs) {
            if (tree.size() != scores.size())
                throw new RuntimeException("tree.size() !=  scores.size() (" + tree.size() + " != " + scores.size() + ")");

            setTree(tree);
            setScores(scores);
            setFreqs(freqs);
        }

        @Override
        public String getType () {
            return NAME;
        }

        @JSON
        public Iterable<Node> getTree () {
            return tree;
        }

        @JSON
        public List<Double> getScores () {
            return scores;
        }

        @JSON
        public List<int[]> getFreqs () {
            return freqs;
        }
    }

    public static VisualizationMessages.Request issueRequest (Tree tree,
                                                              List<Double> scores,
                                                              List<int[]> freqs) {
        return new Request(tree, scores, freqs);
    }

    static {
        Messages.registerRequest(NAME, VisualizationMessages.Request.class);
    }
}