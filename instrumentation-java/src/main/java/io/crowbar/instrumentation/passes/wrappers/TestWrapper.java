package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;

import javassist.CtClass;
import javassist.CtMethod;

public interface TestWrapper extends ActionTaker {
    /*!
     * This function should return the oracle code for expected transactions.
     * Unless specified otherwise, transactions that go to the catch block will fail.
     * To change this behavior, the exceptionVar should be thrown.
     */
    String getOracleCode (CtClass c,
                          CtMethod m,
                          Node n,
                          Probe p,
                          String collectorVar,
                          String exceptionVar);
}