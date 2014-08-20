package io.crowbar.diagnosis.algorithms;

import flexjson.JSON;
import flexjson.JSONSerializer;

import java.util.Map;
import java.util.HashMap;


public final class Algorithm {
    private String name = null;
    private Map<String, String> configs = null;

    /*! Used for JSON deserialization */
    private Algorithm () {}

    /*! Used for JSON deserialization */
    private void setName (String name) {
        this.name = name;
    }

    /*! Used for JSON deserialization */
    private void setConfigs (Map<String, String> configs) {
        this.configs = configs;
    }

    Algorithm (String name,
               Map<String, String> configs) {
        if (name == null)
            throw new NullPointerException();

        this.name = name;

        if (configs != null)
            this.configs = new HashMap<String, String> (configs);
    }

    @JSON
    public String getName () {
        return name;
    }

    @JSON
    private Map<String, String> getConfigs () {
        return configs;
    }

    public String getConfig (String name) {
        return configs.get(name);
    }

    @Override
    public String toString () {
        return new JSONSerializer().exclude("*.class").deepSerialize(this);
    }
}
