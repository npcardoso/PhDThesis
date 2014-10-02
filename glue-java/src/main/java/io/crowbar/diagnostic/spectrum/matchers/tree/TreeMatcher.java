package io.crowbar.diagnostic.spectrum.matchers.tree;

import io.crowbar.diagnostic.spectrum.Tree;

import java.util.BitSet;

public interface TreeMatcher {
    BitSet matchNodes (Tree t);
}