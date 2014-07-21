package io.crowbar.instrumentation.runtime;
import java.util.*;

public enum ProbeType {
    TRANSACTION_START("TransactionStart", "transactionStart"),
    TRANSACTION_END("TransactionEnd", "transactionEnd"),
    HIT_PROBE("HitProbe", "hitprobe"),
    ORACLE("Oracle", "oracle");

    public final String name;
    public final String method_name;
    ProbeType(String name,
              String method_name) {
        this.name = name;
        this.method_name = method_name;
    }
}