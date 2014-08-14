package io.crowbar.diagnosis;

import flexjson.JSON;
import flexjson.JSONSerializer;

import java.util.Map;
import java.util.HashMap;


public final class Algorithm {
    private final String name;
    private Map<String, String> configs;

    Algorithm (String name,
               Map<String, String> configs) {
        if (name == null)
            throw new NullPointerException();

        this.name = name;

        if (configs != null)
            this.configs = new HashMap<String, String> (configs);
    }

    @JSON
    public final String getName () {
        return name;
    }

    @JSON
    private final Map<String, String> getConfigs () {
        return configs;
    }

    public final String getConfig (String name) {
        return configs.get(name);
    }

    @Override
    public String toString () {
        return new JSONSerializer().exclude("*.class").deepSerialize(this);
    }
}