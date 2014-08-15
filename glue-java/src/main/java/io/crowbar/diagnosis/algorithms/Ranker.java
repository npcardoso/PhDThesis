package io.crowbar.diagnosis.algorithms;

import io.crowbar.diagnosis.Algorithm;

public abstract class Ranker {
    Ranker () {}

    public abstract Algorithm getAlgorithm ();
}