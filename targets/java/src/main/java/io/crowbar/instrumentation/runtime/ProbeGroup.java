package io.crowbar.instrumentation.runtime;

public final class ProbeGroup {
    public final class Probe {
        private int globalId;
        private int localId;
        private int nodeId;
        private ProbeType type;
        private Probe (int globalId,
                       int localId,
                       int nodeId,
                       ProbeType type) {
            this.globalId = globalId;
            this.localId = localId;
            this.nodeId = nodeId;
            this.type = type;
        }

        public boolean getActivation () {
            if (hitVector == null)
                return false;

            return hitVector[localId];
        }

        public int getLocalId () {
            return localId;
        }

        public int getGlobalId () {
            return globalId;
        }

        public int getNodeId () {
            return nodeId;
        }

        public ProbeType getType () {
            return type;
        }

        public void hit () {
            assert hitVector != null;
            hitVector[localId] = true;
        }
    }

    private int size = 0;
    private boolean[] hitVector = null;

    ProbeGroup () {}

    public Probe register (int globalId,
                           int nodeId,
                           ProbeType type) {
        assert hitVector == null;
        return new Probe(globalId, size++, nodeId, type);
    }

    public boolean[] get () {
        if (hitVector == null)
            hitVector = new boolean[size];

        return hitVector;
    }

    public void reset () {
        if (hitVector == null)
            return;

        for (int j = 0; j < hitVector.length; j++) {
            hitVector[j] = false;
        }
    }
}