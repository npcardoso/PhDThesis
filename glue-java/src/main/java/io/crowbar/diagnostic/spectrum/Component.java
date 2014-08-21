package io.crowbar.diagnostic.spectrum;

public abstract class Component<Metadata> {
    private final int id;

    Component (int id) {
        this.id = id;
    }

    public final int getId () {
        return id;
    }

    public abstract Metadata getMetadata ();


    @Override
    public final String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Component', ");
        str.append("id=" + getId() + ", ");
        str.append("metadata=" + getMetadata() + "}");

        return str.toString();
    }
}
