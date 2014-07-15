package io.crowbar.instrumentation.runtime;

import java.util.Date;


public class Collector {
    private static Collector collector = new Collector();

    public static Collector getDefault() {
        return collector;
    }


    public void probe (long id) {
        System.out.println("collecting probe: " + id);
    }
// TODO: Probe with state

    public void transaction (long c_id,
                             boolean start) {}
    public void oracle (long c_id,
                        double error,
                        double confidence) {}

    public long registerTransaction() {return 0;}
    public long registerProbe() {return 0;}
    public long registerOracle() {return 0;}

    public void metadata (long c_id,
                          String key,
                          String val) {}
}
