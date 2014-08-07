package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.passes.wrappers.TestWrapper;
import io.crowbar.instrumentation.passes.matchers.ActionTaker.Action;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;

import java.util.Arrays;
import java.util.List;
import java.util.LinkedList;


public final class TestWrapperPass extends AbstractPass {
    private final List<TestWrapper> testWrappers = new LinkedList<TestWrapper> ();


    public TestWrapperPass (TestWrapper... testWrappers) {
        this.testWrappers.addAll(Arrays.asList(testWrappers));
    }

    public TestWrapperPass (List<TestWrapper> testWrappers) {
        this.testWrappers.addAll(testWrappers);
    }

    @Override
    public Outcome transform (CtClass c) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            for (TestWrapper tw : testWrappers) {
                Action ret = tw.getAction(c, m);

                if (ret == Action.ACCEPT) {
                    Node n = getNode(c, m);
                    HitProbe sp = registerProbe(c, n, ProbeType.TRANSACTION_START);
                    HitProbe ep = registerProbe(c, n, ProbeType.TRANSACTION_END);

                    m.insertBefore(getTransactionCode(sp, false));
                    injectOracles(c, m, n, tw, ep);
                    m.insertAfter(getTransactionCode(ep, true),
                                  false /* asFinally */);
                }
                else if (ret == Action.REJECT) {
                    return Outcome.CONTINUE;
                }
            }
        }

        return Outcome.CONTINUE;
    }

    private void injectOracles (CtClass c,
                                CtMethod m,
                                Node n,
                                TestWrapper wrapper,
                                HitProbe transactionEndProbe) throws Exception {
        final String exceptionVar = "eeeeeeeeeee";
        final String collectorVar = "ccccccccccc";
        HitProbe op = registerProbe(c, n, ProbeType.ORACLE);
        StringBuilder code = new StringBuilder();


        /*!
         * The following code generates something like this:
         * {Collector ccccccccccc = Collector.instance();
         *  try {
         *      // ... oracleCode ... throw eeeeeeeeeee; <-- if transaction is a pass should throw.
         *      ccccccccccc.hit(id);
         *      ccccccccccc.oracle(id, 1d, 1d);
         *  }
         *  finally {
         *      ccccccccccc.hit(id);
         *      ccccccccccc.transactionEnd(id,eeeeeeeeeee);
         *      throw eeeeeeeeeee;
         *  }
         *  }
         */
        code.append("{Collector " + collectorVar + " = Collector.instance();");
        code.append("try {");
        String oracleCode = wrapper.getOracleCode(c, m, n, op, collectorVar, exceptionVar);
        code.append((oracleCode != null) ? oracleCode : "");

        // Oracle Fail
        code.append(collectorVar + ".hit(" + op.getId() + ");");
        code.append(collectorVar + "." + op.getType().getMethodName() + "(" + op.getId() + ", 1d, 1d);");

        code.append("} finally {");
        // Transaction End
        code.append(collectorVar + ".hit(" + transactionEndProbe.getId() + ");");
        code.append(collectorVar + "." + transactionEndProbe.getType().getMethodName() + "(");
        code.append(transactionEndProbe.getId() + ",");
        code.append(exceptionVar + ");");

        code.append("throw " + exceptionVar + ";}}");

        CtClass exceptionCls = ClassPool.getDefault().get("java.lang.Throwable");
        m.addCatch(code.toString(), exceptionCls, exceptionVar);
    }

    private String getTransactionCode (HitProbe p,
                                       boolean hitFirst) throws RegistrationException {
        String ret = "Collector c = Collector.instance();";
        String hit = "c.hit(" + p.getId() + ");";


        if (hitFirst)
            ret += hit;

        ret += "c." + p.getType().getMethodName() + "(";
        ret += p.getId();

        ret += ");";

        if (!hitFirst)
            ret += hit;

        return ret;
    }
}