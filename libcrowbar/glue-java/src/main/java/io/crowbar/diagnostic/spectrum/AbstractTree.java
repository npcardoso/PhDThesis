package io.crowbar.diagnostic.spectrum;

import flexjson.JSONSerializer;

public abstract class AbstractTree implements Tree {
    @Override
    public final String toString () {
        return new JSONSerializer().exclude("*.class").deepSerialize(this);
    }
}