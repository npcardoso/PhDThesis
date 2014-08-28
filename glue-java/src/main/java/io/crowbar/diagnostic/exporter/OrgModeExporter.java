package io.crowbar.diagnostic.exporter;

import io.crowbar.diagnostic.Connection;
import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.algorithms.Algorithm;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Tree;

import java.util.Map;


// TODO: Remove this
import io.crowbar.diagnostic.DiagnosticSystemFactory;
import io.crowbar.diagnostic.algorithms.*;
import io.crowbar.diagnostic.spectrum.Activity;


public final class OrgModeExporter {
    private static String GEN_ANCHOR = "gen";
    private static String RANK_ANCHOR = "rank";
    private static String CON_ANCHOR = "con";
    private static String NODE_ANCHOR = "node";
    private static String PROBE_ANCHOR = "prb";
    private static String TRANSACTION_ANCHOR = "tr";

    private int nodeListAtDepth;

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
                          Spectrum< ? , ? > spectrum,
                          DiagnosticReport dr) {
        StringBuilder ret = new StringBuilder();


        header(0, "DiagnosticSystem", ret);
        export(1, ds, ret);
        header(0, "Spectrum", ret);
        export(1, spectrum, ret);

        return ret.toString();
    }

    public void export (int depth,
                        DiagnosticSystem ds,
                        StringBuilder ret) {
        header(depth, "Generators", ret);
        int i = 0;

        for (Algorithm a : ds.getGenerators()) {
            header(depth + 1, "Generator " + i + " " + anchor(GEN_ANCHOR, i), ret);
            export(a, ret);
            i++;
        }

        header(depth, "Rankers", ret);
        i = 0;

        for (Algorithm a : ds.getRankers()) {
            header(depth + 1, "Ranker " + i + " " + anchor(RANK_ANCHOR, i), ret);
            export(a, ret);
            i++;
        }

        header(depth, "Connections", ret);
        i = 0;

        for (Connection c : ds.getConnections()) {
            header(depth + 1, "Connection" + i + " " + anchor(CON_ANCHOR, i), ret);
            listItem(0, "from: " + ds.getGenerators().get(c.getFrom()).getName() + " " +
                     link(GEN_ANCHOR, c.getFrom()), ret);
            listItem(0, "to: " + ds.getRankers().get(c.getTo()).getName() + " " +
                     link(RANK_ANCHOR, c.getTo()), ret);
            i++;
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
                        Spectrum< ? , ? > spectrum,
                        StringBuilder ret) {
        header(depth, "Tree", ret);

        for (Node n : spectrum.getTree().getRoot().getChildren()) {
            export(depth + 1, n, ret);
        }

        header(depth, "Probes", ret);

        for (Probe p : spectrum.byProbe()) {
            if (p != null)
                export(depth + 1, p, ret);
        }

        header(depth, "Transactions", ret);

        for (Transaction< ? , ? > t : spectrum.byTransaction()) {
            if (t != null)
                export(depth + 1, spectrum, t, ret);
        }
    }

    public void export (int depth,
                        Node n,
                        StringBuilder ret) {
        String nodeCaption = n.getName() + " " + anchor(NODE_ANCHOR, n.getId());


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
                        Probe p,
                        StringBuilder ret) {
        header(depth, "Probe " + p.getId() + " " + anchor(PROBE_ANCHOR, p.getId()), ret);
        listItem(0, "node: " + link(NODE_ANCHOR, p.getNodeId()), ret);
        listItem(0, "type: " + p.getType().getName(), ret);
    }

    public void export (int depth,
                        Spectrum spectrum,
                        Transaction< ? , ? > t,
                        StringBuilder ret) {
        header(depth, "Transaction" + t.getId() + " " + anchor(TRANSACTION_ANCHOR, t.getId()), ret);
        listItem(0, "error: " + t.getError(), ret);
        listItem(0, "confidence: " + t.getConfidence(), ret);

        StringBuilder activeNodes = new StringBuilder();
        StringBuilder activeProbes = new StringBuilder();
        int i = 0;

        for (Activity a : t) {
            if (a == null)
                continue;

            if (!a.isActive())
                continue;

            Probe p = spectrum.getProbe(i++);

            if (p == null)
                continue;

            activeProbes.append(" " + link(PROBE_ANCHOR, p.getId()));
            activeNodes.append(" " + link(NODE_ANCHOR, p.getNodeId()));
        }

        listItem(0, "active probes:" + activeProbes.toString(), ret);
        listItem(0, "active nodes:" + activeNodes.toString(), ret);
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
        return " <<" + prefix + id + ">>";
    }

    private String link (String prefix,
                         int id) {
        return "[[" + prefix + id + "]]";
    }

    public static void main (String[] args) {
        DiagnosticSystemFactory j = new DiagnosticSystemFactory();


        j.addGenerator(new SingleFaultGenerator());
        j.addGenerator(new MHSGenerator());

        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        j.addConnection(0, 0);
        j.addConnection(1, 0);

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(1, 1);

        DiagnosticSystem ds = j.create();

        System.out.println(new OrgModeExporter().export(ds, null, null));
    }
}