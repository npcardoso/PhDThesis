package io.crowbar.instrumentation.runtime;

public final class ProbeGroup {
    public final class HitProbe extends Probe {
        private int localId;
        private HitProbe (int globalId,
                          int localId,
                          int nodeId,
                          ProbeType type) {
            super(globalId, nodeId, type);
            this.localId = localId;
        }

        public boolean getActivation () {
            if (hitVector == null)
                return false;

            return hitVector[localId];
        }

        public int getLocalId () {
            return localId;
        }

        public void hit () {
            assert hitVector != null;
            hitVector[localId] = true;
        }
    }

    private int size = 0;
    private boolean[] hitVector = null;

    ProbeGroup () {}

    public HitProbe register (int globalId,
                              int nodeId,
                              ProbeType type) {
        assert hitVector == null;
        return new HitProbe(globalId, size++, nodeId, type);
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