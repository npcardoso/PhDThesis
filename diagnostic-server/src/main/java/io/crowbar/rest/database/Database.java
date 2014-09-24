package io.crowbar.rest.database;

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class Database {
    private final Map<String, SpectrumEntry> sEntries = new HashMap<String, SpectrumEntry> ();
    private final Map<String, DiagnosticEntry> dEntries = new HashMap<String, DiagnosticEntry> ();

    public void handle (String id,
                        SpectrumEntry e) {
        sEntries.put(id, e);
    }

    public Map<String, SpectrumEntry> getSpectra () {
        return Collections.unmodifiableMap(sEntries);
    }

    public void handle (String id,
                        DiagnosticEntry e) {
        dEntries.put(id, e);
    }

    public Map<String, DiagnosticEntry> getDiagnostics () {
        return Collections.unmodifiableMap(dEntries);
    }
}