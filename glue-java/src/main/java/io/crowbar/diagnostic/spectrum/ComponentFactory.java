package io.crowbar.diagnostic.spectrum;


public final class ComponentFactory<CM extends Metadata> {
    private class C extends Component<CM> {
        private final CM metadata;
        C (int id,
           CM metadata) {
            super(id);
            this.metadata = metadata;
        }

        @Override
        public final CM getMetadata () {
            return metadata;
        }
    }

    public Component<CM> create (int id,
                                 CM metadata) {
        return new C(id, metadata);
    }
}
