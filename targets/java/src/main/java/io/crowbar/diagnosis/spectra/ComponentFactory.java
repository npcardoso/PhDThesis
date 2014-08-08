package io.crowbar.diagnosis.spectra;


public final class ComponentFactory<CM extends Metadata> {
    private class C extends Component<CM> {
        private final CM metadata;
        C (CM metadata) {
            this.metadata = metadata;
        }

        @Override
        public final CM getMetadata () {
            return metadata;
        }
    }

    public Component<CM> create (CM metadata) {
        return new C(metadata);
    }
}