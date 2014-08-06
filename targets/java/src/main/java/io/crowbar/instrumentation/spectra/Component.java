package io.crowbar.instrumentation.spectra;


public class Component implements io.crowbar.diagnosis.spectra.Component<ComponentMetadata> {
    private final ComponentMetadata metadata;
    public Component (ComponentMetadata metadata) {
        this.metadata = metadata;
    }

    @Override
    public final ComponentMetadata getMetadata () {
        return metadata;
    }
}