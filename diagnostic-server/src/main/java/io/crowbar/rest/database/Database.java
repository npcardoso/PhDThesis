package io.crowbar.rest.database;

import io.crowbar.rest.models.SessionModel;


import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class Database {
    private final Map<Integer, SessionEntry> sesEntries = new HashMap<Integer, SessionEntry> ();
    private final Map<Integer, SpectrumEntry> specEntries = new HashMap<Integer, SpectrumEntry> ();
    private final Map<Integer, DiagnosticEntry> diagEntries = new HashMap<Integer, DiagnosticEntry> ();

    public synchronized int newSession (String globalId) {
        int sessionId = sesEntries.size();


        sesEntries.put(sessionId, new SessionEntry(new SessionModel(sessionId, globalId)));
        return sessionId;
    }

    public Map<Integer, SessionEntry> getSessions () {
        return Collections.unmodifiableMap(sesEntries);
    }

    public synchronized void handle (int sessionId,
                                     SpectrumEntry e) {
        specEntries.put(sessionId, e);
    }

    public Map<Integer, SpectrumEntry> getSpectra () {
        return Collections.unmodifiableMap(specEntries);
    }

    public synchronized void handle (int sessionId,
                                     DiagnosticEntry e) {
        diagEntries.put(sessionId, e);
    }

    public Map<Integer, DiagnosticEntry> getDiagnostics () {
        return Collections.unmodifiableMap(diagEntries);
    }
}