package io.crowbar.diagnosis.spectra.activity;

import io.crowbar.diagnosis.spectra.Activity;

public final class Hit implements Activity {
    private final boolean active;

    public Hit (boolean active) {
        this.active = active;
    }

    @Override
    public boolean isActive () {
        return active;
    }
}