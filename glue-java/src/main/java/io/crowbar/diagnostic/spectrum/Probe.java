package io.crowbar.diagnostic.spectrum;

public final class Probe {
    private final Spectrum spectrum;
    private final int id;
    private final int nodeId;
    private final ProbeType type;

    Probe (Spectrum spectrum,
           ProbeType type,
           int id) {
        this(spectrum, type, id, 0);
    }

    Probe (Spectrum spectrum,
           ProbeType type,
           int id,
           int nodeId) {
        this.spectrum = spectrum;
        this.type = type;
        this.id = id;
        this.nodeId = nodeId;
    }

    public ProbeType getType () {
        return type;
    }

    public int getId () {
        return id;
    }

    public int getNodeId () {
        return nodeId;
    }

    public Node getNode () {
        return spectrum.getTree().getNode(nodeId);
    }

    @Override
    public String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Probe', ");
        str.append("id=" + getId() + ", ");
        str.append("nodeId=" + getNodeId() + ",");
        str.append("type=" + getType().getName() + "}");

        return str.toString();
    }
}