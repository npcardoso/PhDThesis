package io.crowbar.instrumentation.runtime;

public enum ProbeType {
    TRANSACTION_START("TransactionStart", "transactionStart"),
    TRANSACTION_END("TransactionEnd", "transactionEnd"),
    HIT_PROBE("HitProbe", "hitprobe"),
    ORACLE("Oracle", "oracle");

    private final String name;
    private final String methodName;
    ProbeType(String name,
              String methodName) {
        this.name = name;
        this.methodName = methodName;
    }
    
    public final String getName() {
    	return name;
    }
    
    public final String getMethodName() {
    	return methodName;
    }
}