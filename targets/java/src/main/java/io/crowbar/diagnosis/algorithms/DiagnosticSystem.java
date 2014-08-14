package io.crowbar.diagnosis.algorithms;

import io.crowbar.diagnosis.spectra.Spectra;

import flexjson.JSON;
import flexjson.JSONSerializer;
import flexjson.transformer.AbstractTransformer;

import java.util.ArrayList;
import java.util.List;
import io.crowbar.diagnosis.spectra.EditableSpectra;
import io.crowbar.diagnosis.spectra.serializers.HitSpectraSerializer;

public final class DiagnosticSystem {
    private static class ExcludeTransformer extends AbstractTransformer {
        @Override
        public Boolean isInline () {return true;}

        @Override
        public void transform (Object object) {}
    }

    private static class Connection {
        private int from;
        private int to;
        Connection (int from, int to) {
            this.from = from;
            this.to = to;
        }

        private int getFrom () {
            return from;
        }

        private int getTo () {
            return to;
        }
    }

    private final List<Algorithm> generators = new ArrayList<Algorithm> ();
    private final List<Algorithm> rankers = new ArrayList<Algorithm> ();
    private final List<Connection> connections = new ArrayList<Connection> ();

    // ! Adds a generator to the diagnostic system. (Note: makes a copy of the generator so its safe to reuse it.)
    public int addGenerator (Generator g) {
        generators.add(new Algorithm(g.getAlgorithm()));
        return generators.size() - 1;
    }

    // ! Adds a ranker to the diagnostic system. (Note: makes a copy of the ranker so its safe to reuse it.)
    public int addRanker (Ranker r) {
        rankers.add(new Algorithm(r.getAlgorithm()));
        return rankers.size() - 1;
    }

    public void addConnection (int generatorId,
                               int rankerId) {
        assert generatorId >= 0 && generatorId < generators.size();
        assert rankerId >= 0 && rankerId < rankers.size();
        connections.add(new Connection(generatorId, rankerId));
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

    public void run (Spectra spectra) {
        String ret = this.run(HitSpectraSerializer
                              .serialize(spectra),
                              this.toString());
    }

    public native String run (String spectra, String configs);


    @Override
    public String toString () {
        return new JSONSerializer()
               .transform(new ExcludeTransformer(), void.class)
               .exclude("*.class")
               .deepSerialize(this);
    }

    public static void main (String[] args) {
        DiagnosticSystem j = new DiagnosticSystem();


        j.addGenerator(new SingleFaultGenerator());
        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        j.addConnection(0, 0);

        j.addGenerator(new MHSGenerator());
        j.addRanker(new FuzzinelRanker());
        j.addConnection(0, 1);
        j.addConnection(1, 1);

        // j.run(new EditableSpectra());
        System.out.println("DiagnosticSystem: " + j);
    }
}