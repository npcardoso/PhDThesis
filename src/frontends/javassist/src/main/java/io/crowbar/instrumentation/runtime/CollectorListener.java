package io.crowbar.instrumentation.runtime;


public interface CollectorListener {
    public void startTransaction (Collector c,
                                  Probe p);

    public void endTransaction (Collector c,
                                Probe p,
                                boolean[] hit_vector);

    public void oracle (Collector c,
                        Probe p,
                        double error,
                        double confidence);
}