package io.crowbar.diagnosis;

import io.crowbar.diagnosis.algorithms.Algorithm;

import flexjson.JSON;

import java.util.ArrayList;
import java.util.List;

/**
 * This class represents a diagnostic system.  As this class is
 * immutable, a DiagnosticSystemFactory should be used to create
 * instances of this class.
 */
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
        setGenerators(new ArrayList<Algorithm> (generators));
        setRankers(new ArrayList<Algorithm> (rankers));
        setConnections(new ArrayList<Connection> (connections));
    }

    /**
     * \brief Returns the list of generators.
     * \return An unmodifiable List containing all generators.
     */
    @JSON
    public List<Algorithm> getGenerators () {
        return java.util.Collections.unmodifiableList(generators);
    }

    /**
     * \brief Returns the list of rankers.
     * \return An unmodifiable List containing all rankers.
     */
    @JSON
    public List<Algorithm> getRankers () {
        return java.util.Collections.unmodifiableList(rankers);
    }


    /**
     * \brief Returns the list of connections.
     * \return An unmodifiable List containing all connections.
     */
    @JSON
    public List<Connection> getConnections () {
        return java.util.Collections.unmodifiableList(connections);
    }
}
