package io.crowbar.diagnosis.algorithms;

import flexjson.JSON;
import flexjson.JSONSerializer;

import java.util.Map;
import java.util.HashMap;


class Algorithm {
    private final String name;
    private Map<String, String> configs = null;

    Algorithm (String name) {
        this.name = name;
    }

    Algorithm (Algorithm algorithm) {
        this.name = algorithm.name;

        if (algorithm.configs != null)
            configs = new HashMap<String, String> (algorithm.configs);
    }

    @JSON
    public final String getName () {
        return name;
    }

    @JSON
    private final Map<String, String> getConfigs () {
        return configs;
    }

    protected final void setConfig (String name, String value) {
        if (configs == null)
            configs = new HashMap<String, String> ();

        configs.put(name, value);
    }

    public final String getConfig (String name) {
        return configs.get(name);
    }

    @Override
    public String toString () {
        return new JSONSerializer().exclude("*.class").deepSerialize(this);
    }
}