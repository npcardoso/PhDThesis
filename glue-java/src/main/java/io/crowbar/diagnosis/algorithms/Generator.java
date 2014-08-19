package io.crowbar.diagnosis.algorithms;

import io.crowbar.diagnosis.Algorithm;

public abstract class Generator {
    Generator () {}

    public abstract Algorithm getAlgorithm ();
}
