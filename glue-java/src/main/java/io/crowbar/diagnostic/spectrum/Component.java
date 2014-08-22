package io.crowbar.diagnostic.spectrum;

public final class Component {
    private final Spectrum spectrum;
    private final int id;
    private final int nodeId;
    private final ProbeType type;

    Component (Spectrum spectrum,
               ProbeType type,
               int id) {
        this(spectrum, type, id, 0);
    }

    Component (Spectrum spectrum,
               ProbeType type,
               int id,
               int nodeId) {
        this.spectrum = spectrum;
        this.type = type;
        this.id = id;
        this.nodeId = nodeId;
    }

    public final ProbeType getType() {
        return type;
    }

    public final int getId () {
        return id;
    }

    public final int getNodeId () {
        return nodeId;
    }

    public final Node getNode () {
        return spectrum.getTree().getNode(nodeId);
    }

    @Override
    public final String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Component', ");
        str.append("id=" + getId() + ", ");
        str.append("nodeId=" + getNodeId() + ", ");

        return str.toString();
    }
}
