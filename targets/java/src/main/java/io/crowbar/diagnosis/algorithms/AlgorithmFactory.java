package io.crowbar.diagnosis.algorithms;

import flexjson.JSONSerializer;
import flexjson.JSON;

import java.util.Map;
import java.util.HashMap;

public class AlgorithmFactory {
    public static class Algorithm {
        private final String name;
        private final Map<String, String> configs;

        protected Algorithm (String name,
                             Map<String, String> configs) {
            this.name = name;
            this.configs = new HashMap<String, String> (configs);
        }

        private String getName () {
            return name;
        }

        private Map<String, String> getConfigs () {
            return configs;
        }

        @Override
        public String toString () {
            return new JSONSerializer().serialize(this);
        }
    }

    private final Map<String, String> configs = new HashMap<String, String> ();

    protected final void addConfig (String name, String value) {
        configs.put(name, value);
    }

    public final Algorithm create (String name) {
        return new Algorithm(name, configs);
    }
}