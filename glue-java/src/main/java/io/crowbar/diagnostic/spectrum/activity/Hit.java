package io.crowbar.diagnostic.spectrum.activity;

import io.crowbar.diagnostic.spectrum.Activity;

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
