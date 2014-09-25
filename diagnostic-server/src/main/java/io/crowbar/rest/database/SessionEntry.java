package io.crowbar.rest.database;

import io.crowbar.rest.models.SessionModel;

public final class SessionEntry {
    private final SessionModel ses;

    public SessionEntry (SessionModel ses) {
        this.ses = ses;
    }

    public SessionModel getSession () {
        return ses;
    }
}