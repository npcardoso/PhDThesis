package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;

import javassist.CtClass;
import javassist.CtMethod;

public interface TestWrapper extends ActionTaker {
    /**
     * @brief This function should return the oracle code for expected transactions.
     * Unless specified otherwise, transactions that enter the catch block will fail.
     * To change this behavior, the exceptionVar should be thrown.
     * The generated code will be something similar to:
     *
     * {Collector c = Collector.instance();
     *  try {
     *      // ... oracleCode ... throw e; <-- if transaction is a pass 'e' should be thrown.
     *      c.hit(id);
     *      c.oracle(id, 1d, 1d);
     *  }
     *  finally {
     *      c.hit(id);
     *      c.transactionEnd(id,eeeeeeeeeee);
     *      throw eeeeeeeeeee;
     *  }
     *  }
     */

    String getOracleCode (CtClass c,
                          CtMethod m,
                          Node n,
                          int probeId,
                          String collectorVar,
                          String exceptionVar);

    boolean isDefaultPass (CtClass c,
                           CtMethod m);
}