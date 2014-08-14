package io.crowbar.diagnosis;

import java.util.Map;
import java.util.HashMap;


public final class AlgorithmFactory {
    private Map<String, String> configs = null;

    public void setConfig (String name, String value) {
        if (configs == null)
            configs = new HashMap<String, String> ();

        configs.put(name, value);
    }

    public String getConfig (String name) {
        if (configs == null)
            return null;

        return configs.get(name);
    }

    public void removeConfig (String name) {
        if (configs == null)
            return;

        configs.remove(name);
    }

    public Algorithm create (String name) {
        return new Algorithm(name, configs);
    }
}