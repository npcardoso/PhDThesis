package io.crowbar.diagnosis.spectra;


import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;


public class ComponentFactory<CM extends Metadata> {
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