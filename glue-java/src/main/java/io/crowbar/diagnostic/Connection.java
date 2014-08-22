package io.crowbar.diagnostic;

import flexjson.JSON;

/**
 * @bug Id is 0 after deserialization.
 */
public final class Connection {
    private int id;
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

    Connection (int id,
                int from,
                int to) {
        this.id = id;
        this.from = from;
        this.to = to;
    }

    @JSON(include=false)
    public int getId() {
        return id;
    }

    public int getFrom () {
        return from;
    }

    public int getTo () {
        return to;
    }

    @Override
    public String toString() {
        return getClass().getName() + "[id:" + id + ",from:" + from + ",to:" + to + "]";
    }
}
