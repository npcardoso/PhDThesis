package io.crowbar.diagnosis.spectra;

public abstract class Component<Metadata> {
    Component () {}

    public abstract Metadata getMetadata ();
}