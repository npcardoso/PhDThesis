package io.crowbar.diagnosis.spectra;

public final class HitActivity implements Activity {
    private final boolean active;

    public HitActivity (boolean active) {
        this.active = active;
    }

    @Override
    public boolean isActive () {
        return active;
    }
}