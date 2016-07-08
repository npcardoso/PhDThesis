package io.crowbar.util;

import java.util.List;

public interface MergeStrategy {
    double reduce (List<Double> scores);

    public static final MergeStrategy AVG =
        new MergeStrategy () {
        @Override
        public double reduce (List<Double> scores) {
            if (scores.size() <= 0) return Double.NaN;

            double total = 0;

            for (Double s : scores) {
                total += s;
            }

            return total / scores.size();
        }
    };

    public static final MergeStrategy MAX =
        new MergeStrategy () {
        @Override
        public double reduce (List<Double> scores) {
            if (scores.size() <= 0) return Double.NaN;

            double max = 0;

            for (Double s : scores) {
                max = Math.max(max, s);
            }

            return max;
        }
    };

    public static final MergeStrategy SUM =
        new MergeStrategy () {
        @Override
        public double reduce (List<Double> scores) {
            if (scores.size() <= 0) return Double.NaN;

            double total = 0;

            for (Double s : scores) {
                total += s;
            }

            return total;
        }
    };
}