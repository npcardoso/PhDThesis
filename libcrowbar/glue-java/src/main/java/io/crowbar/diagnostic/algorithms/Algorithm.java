package io.crowbar.diagnostic.algorithms;

import flexjson.JSON;
import flexjson.JSONSerializer;

import java.util.Collections;
import java.util.Map;
import java.util.HashMap;

/**
 * \brief This class represents an algorithm with optional configurations.
 * The class is mainly used for serialization.
 * The class is immutable.
 */
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

    /**
     * This constructor creates a copy of the configurations to ensure
     * immutability.
     */
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
    public Map<String, String> getConfigs () {
        return configs == null ? null : Collections.unmodifiableMap(configs);
    }

    public String getConfig (String name) {
        return configs.get(name);
    }

    @Override
    public String toString () {
        return new JSONSerializer().exclude("*.class").deepSerialize(this);
    }
}