package io.crowbar.rest.database;

import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.rest.models.SpectrumModel;
import io.crowbar.rest.models.DiagnosticReportModel;


import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.ArrayList;

public final class Database {
    public static class Table<T> {
        private final List<T> elements = new ArrayList<T> ();
        public synchronized int add (T e) {
            elements.add(e);
            return elements.size() - 1;
        }

        public T get (int id) {
            if (id < 0 || id >= elements.size())
                return null;

            return elements.get(id);
        }

        public List<T> elements () {
            return Collections.unmodifiableList(elements);
        }
    }


    private final Table<SessionEntry> sessions = new Table<SessionEntry> ();
    private final Table<DiagnosticSystem> diagnosticSystems = new Table<DiagnosticSystem> ();
    private final Table<SpectrumModel> spectra = new Table<SpectrumModel> ();
    private final Table<DiagnosticReportModel> diagnosticReports = new Table<DiagnosticReportModel> ();

    public Table<SessionEntry> getSessions () {
        return sessions;
    }

    public Table<DiagnosticSystem> getDiagnosticSystems () {
        return diagnosticSystems;
    }

    public Table<SpectrumModel> getSpectra () {
        return spectra;
    }

    public Table<DiagnosticReportModel> getDiagnosticReports () {
        return diagnosticReports;
    }
}