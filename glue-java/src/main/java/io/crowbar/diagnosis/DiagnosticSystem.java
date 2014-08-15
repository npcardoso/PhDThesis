package io.crowbar.diagnosis;


import flexjson.JSON;

import java.util.ArrayList;
import java.util.List;

public final class DiagnosticSystem {
    private List<Algorithm> generators = null;
    private List<Algorithm> rankers = null;
    private List<Connection> connections = null;

    /*! Used for JSON deserialization */
    private DiagnosticSystem () {}

    /*! Used for JSON deserialization */
    private void setGenerators (List<Algorithm> generators) {
        this.generators = generators;
    }

    /*! Used for JSON deserialization */
    private void setRankers (List<Algorithm> rankers) {
        this.rankers = rankers;
    }

    /*! Used for JSON deserialization */
    private void setConnections (List<Connection> connections) {
        this.connections = connections;
    }

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