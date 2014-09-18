package io.crowbar.diagnostic.exporter;

import io.crowbar.diagnostic.Candidate;
import io.crowbar.diagnostic.Connection;
import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.DiagnosticElement;
import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.SortedDiagnostic;
import io.crowbar.diagnostic.UnsortedDiagnostic;
import io.crowbar.diagnostic.algorithms.Algorithm;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.diagnostic.spectrum.serializers.HitSpectrumTableSerializer;

import java.util.Map;
import java.util.List;
import java.util.ArrayList;


// TODO: Remove this
import io.crowbar.diagnostic.DiagnosticSystemFactory;
import io.crowbar.diagnostic.algorithms.*;
import io.crowbar.diagnostic.runners.*;
import io.crowbar.util.*;


public final class OrgModeExporter {
    private static String GEN_ANCHOR = "Generator";
    private static String RANK_ANCHOR = "Ranker";
    private static String CON_ANCHOR = "Connection";
    private static String NODE_ANCHOR = "Node";
    private static String PROBE_ANCHOR = "Probe";
    private static String TRANSACTION_ANCHOR = "Transaction";

    private int nodeListAtDepth = -1;

    /**
     * @brief Gets the depth after which tree nodes start to be displayed as list items instead of sections.
     * If depth < 0, sections are always used.
     */
    public int getNodeListAtDepth () {
        return nodeListAtDepth;
    }

    /**
     * @brief Sets the depth after which tree nodes start to be displayed as list items instead of sections.
     * If depth < 0, sections are always used.
     */
    public void setNodeListAtDepth (int depth) {
        this.nodeListAtDepth = depth;
    }

    public String export (DiagnosticSystem ds,
                          Spectrum spectrum,
                          DiagnosticReport dr) {
        int depth = 0;
        StringBuilder ret = new StringBuilder();


        header(depth, "Rankings", ret);
        exportSingleRankings(depth + 1, ds, spectrum, dr, ret);
        header(depth, "Node Scores", ret);
        exportNodeScores(depth + 1, ds, spectrum, dr, ret);
        header(depth, "Raw Data", ret);
        export(depth + 1, ds, ret);
        export(depth + 1, ds, dr, ret);
        exportHitSpectrum(depth + 1, spectrum, ret);
        export(depth + 1, spectrum, ret);

        return ret.toString();
    }

    public void exportSingleRankings (int depth,
                                      DiagnosticSystem ds,
                                      Spectrum spectrum,
                                      DiagnosticReport dr,
                                      StringBuilder ret) {
        int treeSize = spectrum.getTree().size();


        for (Connection c : ds.getConnections()) {
            header(depth, "Ranking for " + link(anchor(CON_ANCHOR, c.getId())), ret);
            Diagnostic d = dr.getDiagnostic(c);
            List<Double> scores = spectrum.getScorePerNode(d, Spectrum.MAX);
            List<Candidate> candidates = new ArrayList<Candidate> (treeSize);

            for (int i = 0; i < treeSize; i++) {
                candidates.add(new Candidate(i));
            }

            d = new SortedDiagnostic(
                new UnsortedDiagnostic(candidates, scores));

            for (DiagnosticElement de : d) {
                if (Double.isNaN(de.getScore()))
                    continue;

                Node n = spectrum.getTree().getNode(de.getCandidate().iterator().next());

                listItem(0, n.getFullName() + " : " + de.getScore(), ret);
            }
        }
    }

    public void exportNodeScores (int depth,
                                  DiagnosticSystem ds,
                                  Spectrum spectrum,
                                  DiagnosticReport dr,
                                  StringBuilder ret) {
        for (Connection c : ds.getConnections()) {
            header(depth, "Scores for " + link(anchor(CON_ANCHOR, c.getId())), ret);
            Diagnostic d = dr.getDiagnostic(c);
            List<Double> scores = spectrum.getScorePerNode(d, Spectrum.MAX);
            export(0, spectrum.getTree().getRoot(), scores, ret);
        }
    }

    public void exportHitSpectrum (int depth,
                                   Spectrum spectrum,
                                   StringBuilder ret) {
        header(depth, "Spectrum", ret);
        String tab = HitSpectrumTableSerializer.serialize(
            spectrum,
            new HitSpectrumTableSerializer.OrgModeTableStyle());
        ret.append(tab);
    }

    public void export (int depth,
                        DiagnosticSystem ds,
                        StringBuilder ret) {
        header(depth, "Diagnostic System", ret);
        header(depth + 1, "Generators", ret);
        int i = 0;

        for (Algorithm a : ds.getGenerators()) {
            header(depth + 2, anchor(GEN_ANCHOR, i), ret);
            export(a, ret);
            i++;
        }

        header(depth + 1, "Rankers", ret);
        i = 0;

        for (Algorithm a : ds.getRankers()) {
            header(depth + 2, anchor(RANK_ANCHOR, i), ret);
            export(a, ret);
            i++;
        }

        header(depth + 1, "Connections", ret);

        for (Connection c : ds.getConnections()) {
            header(depth + 2, anchor(CON_ANCHOR, c.getId()), ret);
            listItem(0, "from: " + link(anchor(GEN_ANCHOR, c.getFrom())), ret);
            listItem(0, "to: " + link(anchor(RANK_ANCHOR, c.getTo())), ret);
        }
    }

    private void export (Algorithm a,
                         StringBuilder ret) {
        listItem(0, "name: " + a.getName(), ret);

        if (a.getConfigs() != null) {
            listItem(0, "configs:", ret);

            for (Map.Entry<String, String> entry : a.getConfigs().entrySet()) {
                listItem(1, entry.getKey() + ": " + entry.getValue(), ret);
            }
        }
    }

    public void export (int depth,
                        Spectrum spectrum,
                        StringBuilder ret) {
        header(depth, "Spectrum", ret);
        header(depth + 1, "Tree", ret);

        for (Node n : spectrum.getTree().getRoot().getChildren()) {
            export(depth + 2, n, ret);
        }

        header(depth + 1, "Probes", ret);

        for (ProbeType t : ProbeType.values()) {
            header(depth + 2, t.getName(), ret);

            for (Probe p : spectrum.byProbe()) {
                if (p != null && p.getType() == t)
                    export(depth + 3, p, ret);
            }
        }

        header(depth + 1, "Transactions", ret);

        for (Transaction t : spectrum.byTransaction()) {
            if (t != null)
                export(depth + 2, spectrum, t, ret);
        }
    }

    public void export (int depth,
                        Node n,
                        StringBuilder ret) {
        String nodeCaption = label(anchor(NODE_ANCHOR, n.getId())) + " : " + n.getName();


        if (nodeListAtDepth >= 0 &&
            depth > nodeListAtDepth)
            listItem(depth - nodeListAtDepth, nodeCaption, ret);
        else
            header(depth, nodeCaption, ret);

        for (Node child : n.getChildren()) {
            export(depth + 1, child, ret);
        }
    }

    public void export (int depth,
                        Node n,
                        List<Double> scores,
                        StringBuilder ret) {
        String nodeCaption = link(anchor(NODE_ANCHOR, n.getId()), n.getName());
        double score = scores.get(n.getId());


        if (!Double.isNaN(score))
            nodeCaption += " : " + score;


        listItem(depth, nodeCaption, ret);

        for (Node child : n.getChildren()) {
            export(depth + 1, child, scores, ret);
        }
    }

    public void export (int depth,
                        Probe p,
                        StringBuilder ret) {
        listItem(0, label(anchor(PROBE_ANCHOR, p.getId())) + " @ " + link(anchor(NODE_ANCHOR, p.getNodeId())), ret);
    }

    public void export (int depth,
                        Spectrum spectrum,
                        Transaction t,
                        StringBuilder ret) {
        header(depth, anchor(TRANSACTION_ANCHOR, t.getId()), ret);
        listItem(0, "error: " + t.getError(), ret);
        listItem(0, "confidence: " + t.getConfidence(), ret);

        StringBuilder activeNodes = new StringBuilder();
        StringBuilder activeProbes = new StringBuilder();

        for (Integer i : t) {
            Probe p = spectrum.getProbe(i);

            if (p == null)
                continue;

            listItem(1, link(anchor(PROBE_ANCHOR, p.getId())), activeProbes);
            listItem(1, link(anchor(NODE_ANCHOR, p.getNodeId())), activeNodes);
        }

        listItem(0, "active probes:", ret);
        ret.append(activeProbes.toString());
        listItem(0, "active nodes:", ret);
        ret.append(activeNodes.toString());
    }

    public void export (int depth,
                        DiagnosticSystem ds,
                        DiagnosticReport dr,
                        StringBuilder ret) {
        header(depth, "Diagnostic Report", ret);

        for (Connection c : ds.getConnections()) {
            header(depth + 1, "Report for " + link(anchor(CON_ANCHOR, c.getId())), ret);
            export(depth + 2, dr.getDiagnostic(c), ret);
        }
    }

    public void export (int depth,
                        Diagnostic d,
                        StringBuilder ret) {
        SortedDiagnostic sd = new SortedDiagnostic(d);


        for (DiagnosticElement de : sd) {
            StringBuilder entry = new StringBuilder(de.getScore() + ":");

            for (int id : de.getCandidate())
                entry.append(" " + link(anchor(PROBE_ANCHOR, id)));

            listItem(0, entry.toString(), ret);
        }
    }

    private void header (int depth,
                         String text,
                         StringBuilder ret) {
        while ((depth--) > 0)
            ret.append("*");

        ret.append("* " + text + "\n");
    }

    private void listItem (int depth,
                           String text,
                           StringBuilder ret) {
        while ((depth--) > 0)
            ret.append(" ");

        ret.append(" - " + text + "\n");
    }

    private String anchor (String prefix,
                           int id) {
        return prefix + " " + id;
    }

    private String label (String lbl) {
        return "<<" + lbl + ">>";
    }

    private String link (String dest) {
        return "[[" + dest + "]]";
    }

    private String link (String dest,
                         String lbl) {
        return "[[" + dest + "][" + lbl + "]]";
    }

    public static void main (String[] args) {
        DiagnosticSystemFactory j = new DiagnosticSystemFactory();


        j.addGenerator(new SingleFaultGenerator());
        j.addGenerator(new MHSGenerator());

        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        j.addConnection(0, 0);
        j.addConnection(1, 0);

        j.addRanker(new FuzzinelRanker());
        j.addConnection(1, 1);

        Spectrum s = SpectraGenerator.generateSpectrum(10, 20, 10, 0.5, 0.5);


        DiagnosticSystem ds = j.create();
        try {
            JNARunner runner = new JNARunner();
            DiagnosticReport dr = runner.run(ds, s);

            System.out.println(new OrgModeExporter().export(ds, s, dr));
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}