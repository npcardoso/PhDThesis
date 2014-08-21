package io.crowbar.diagnostic;

public final class Connection {
    private int from;
    private int to;

    /*! Used for JSON deserialization */
    private Connection () {}

    /*! Used for JSON deserialization */
    private void setFrom (int from) {
        this.from = from;
    }

    /*! Used for JSON deserialization */
    private void setTo (int to) {
        this.to = to;
    }

    public Connection (int from, int to) {
        this.from = from;
        this.to = to;
    }

    public int getFrom () {
        return from;
    }

    public int getTo () {
        return to;
    }
}
