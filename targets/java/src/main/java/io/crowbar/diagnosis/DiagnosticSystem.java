package io.crowbar.diagnosis;


import flexjson.JSON;
import flexjson.JSONSerializer;
import flexjson.transformer.AbstractTransformer;

import java.util.ArrayList;
import java.util.List;

public final class DiagnosticSystem {
    private final List<Algorithm> generators;
    private final List<Algorithm> rankers;
    private final List<Connection> connections;

    DiagnosticSystem (List<Algorithm> generators,
                      List<Algorithm> rankers,
                      List<Connection> connections) {
        this.generators = new ArrayList<Algorithm> (generators);
        this.rankers = new ArrayList<Algorithm> (rankers);
        this.connections = new ArrayList<Connection> (connections);
    }

    @JSON
    private List<Algorithm> getGenerators () {
        return generators;
    }

    @JSON
    private List<Algorithm> getRankers () {
        return rankers;
    }

    @JSON
    private List<Connection> getConnections () {
        return connections;
    }
}