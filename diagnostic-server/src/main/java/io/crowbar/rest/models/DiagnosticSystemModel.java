package io.crowbar.rest.models;

import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.Connection;
import io.crowbar.diagnostic.algorithms.Algorithm;

import java.util.AbstractList;
import java.util.Collections;
import java.util.List;

public final class DiagnosticSystemModel {
    private final DiagnosticSystem original;

    public DiagnosticSystemModel (DiagnosticSystem original) {
        this.original = original;
    }

    private static List<AlgorithmModel> createWrapper (List<Algorithm> original) {
        ListWrapper.WrapperFactory<Algorithm, AlgorithmModel> factory =
            new ListWrapper.WrapperFactory<Algorithm, AlgorithmModel> () {
            @Override
            public AlgorithmModel create (Algorithm original) {
                return new AlgorithmModel(original);
            }
        };

        return new ListWrapper<Algorithm, AlgorithmModel> (original,
                                                           factory);
    }

    public List<AlgorithmModel> getGenerators () {
        return createWrapper(original.getGenerators());
    }

    public List<AlgorithmModel> getRankers () {
        return createWrapper(original.getRankers());
    }

    public List<ConnectionModel> getConnections () {
        ListWrapper.WrapperFactory<Connection, ConnectionModel> factory =
            new ListWrapper.WrapperFactory<Connection, ConnectionModel> () {
            @Override
            public ConnectionModel create (Connection original) {
                return new ConnectionModel(original);
            }
        };

        return new ListWrapper<Connection, ConnectionModel> (original.getConnections(),
                                                             factory);
    }
}