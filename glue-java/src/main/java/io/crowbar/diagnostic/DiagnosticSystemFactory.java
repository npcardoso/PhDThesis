package io.crowbar.diagnostic;

import io.crowbar.diagnostic.algorithms.Algorithm;
import io.crowbar.diagnostic.algorithms.Generator;
import io.crowbar.diagnostic.algorithms.Ranker;

import java.util.ArrayList;
import java.util.List;

/**
 * This class is used to create DiagnosticSystem instances.
 */
public final class DiagnosticSystemFactory {
    private final List<Algorithm> generators = new ArrayList<Algorithm> ();
    private final List<Algorithm> rankers = new ArrayList<Algorithm> ();
    private final List<Connection> connections = new ArrayList<Connection> ();

    /**
     * \brief Adds a generator to the diagnostic system.
     * \note The generator is copied so its safe to reuse it.
     * \return Returns the number of generators
     */
    public int addGenerator (Generator g) {
        generators.add(g.getAlgorithm());
        return generators.size() - 1;
    }

    /**
     * \brief Adds a ranker to the diagnostic system.
     * \note The ranker is copied so its safe to reuse it.
     * \return Returns the number of rankers
     */
    public int addRanker (Ranker r) {
        rankers.add(r.getAlgorithm());
        return rankers.size() - 1;
    }

    /**
     * \brief Adds a connection from a generator to a ranker.
     * \return The connection.
     */
    public Connection addConnection (int from,
                                     int to) {
        if (from < 0 || to >= generators.size())
            throw new IndexOutOfBoundsException();

        if (to < 0 || to >= rankers.size())
            throw new IndexOutOfBoundsException();

        Connection c = new Connection(connections.size(), from, to);
        connections.add(c);
        return c;
    }

    public DiagnosticSystem create () {
        // TODO: Optimize this by caching stuff
        return new DiagnosticSystem(generators,
                                    rankers,
                                    connections);
    }
}
