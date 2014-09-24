package io.crowbar.rest.models;

import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.Connection;
import io.crowbar.diagnostic.algorithms.Algorithm;

import java.util.AbstractList;
import java.util.Collections;
import java.util.List;

public final class DiagnosticSystemModel {
    private static class AlgorithmListWrapper
    extends AbstractList<AlgorithmModel> {
        private final List<Algorithm> original;

        AlgorithmListWrapper (List<Algorithm> original) {
            this.original = original;
        }

        @Override
        public int size () {
            return original.size();
        }

        @Override
        public AlgorithmModel get (int i) {
            Algorithm a = original.get(i);


            if (a == null)
                return null;

            return new AlgorithmModel(a);
        }
    }

    private static class ConnectionListWrapper
    extends AbstractList<ConnectionModel> {
        private final List<Connection> original;

        ConnectionListWrapper (List<Connection> original) {
            this.original = original;
        }

        @Override
        public int size () {
            return original.size();
        }

        @Override
        public ConnectionModel get (int i) {
            Connection c = original.get(i);


            if (c == null)
                return null;

            return new ConnectionModel(c);
        }
    }

    private final DiagnosticSystem original;

    public DiagnosticSystemModel (DiagnosticSystem original) {
        this.original = original;
    }

    public List<AlgorithmModel> getGenerators () {
        return new AlgorithmListWrapper(original.getGenerators());
    }

    public List<AlgorithmModel> getRankers () {
        return new AlgorithmListWrapper(original.getRankers());
    }

    public List<ConnectionModel> getConnections () {
        return new ConnectionListWrapper(original.getConnections());
    }
}