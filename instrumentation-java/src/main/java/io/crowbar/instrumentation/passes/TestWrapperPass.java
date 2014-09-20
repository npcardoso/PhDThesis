package io.crowbar.instrumentation.passes;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.passes.wrappers.TestWrapper;
import io.crowbar.instrumentation.passes.matchers.ActionTaker.Action;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;

import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;
import javassist.bytecode.CodeAttribute;
import javassist.bytecode.MethodInfo;

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
            MethodInfo info = m.getMethodInfo();
            CodeAttribute ca = info.getCodeAttribute();

            if (ca == null)
                continue;

            for (TestWrapper tw : testWrappers) {
                Action ret = tw.getAction(c, m);

                if (ret == Action.ACCEPT) {
                    ca.computeMaxStack();
                    Node n = getNode(c, m);
                    HitProbe sp = registerProbe(c, n, ProbeType.TRANSACTION_START);
                    HitProbe ep = registerProbe(c, n, ProbeType.TRANSACTION_END);
                    HitProbe op = registerProbe(c, n, ProbeType.ORACLE);

                    m.insertBefore(getTransactionStartCode(sp));
                    ca.computeMaxStack();

                    injectOracles(c, m, n, tw, ep, op);
                    ca.computeMaxStack();

                    m.insertAfter(getTransactionEndCode(ep),
                                  true);
                    ca.computeMaxStack();
                } else if (ret == Action.REJECT) {
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
                                HitProbe transactionEndProbe,
                                HitProbe oracleProbe) throws Exception {
        assert (transactionEndProbe.getType() == ProbeType.TRANSACTION_END);

        final String exceptionVar = "eeeeeeeeeee";
        final String collectorVar = "ccccccccccc";
        StringBuilder code = new StringBuilder();
        String oracleCode = wrapper.getOracleCode(c, m, n, oracleProbe.getId(),
                                                  collectorVar, exceptionVar);

        /*!
         * The following code generates something like this:
         * {Collector ccccccccccc = Collector.instance();
         *      // ... oracleCode ... // if transaction is a pass, throw eeeeeeeeeee.
         *      ccccccccccc.hit(id);
         *      ccccccccccc.oracle(id, 1d, 1d);
         *      throw eeeeeeeeeee;
         *  }
         */

        code.append("{Collector " + collectorVar + " = Collector.instance();");

        if (oracleCode != null)
            code.append(oracleCode);

        code.append(collectorVar + ".hit(" + oracleProbe.getId() + ");");
        code.append(collectorVar + "." + oracleProbe.getType().getMethodName() +
                    "(" + oracleProbe.getId() + ", 1d, 1d);");
        code.append("throw " + exceptionVar + ";}");

        CtClass exceptionCls = ClassPool.getDefault().get("java.lang.Throwable");
        m.addCatch(code.toString(), exceptionCls, exceptionVar);

        String quietCode = wrapper.getOracleCode(c, m, n, oracleProbe.getId(), collectorVar);

        if (quietCode != null) {
            code = new StringBuilder();
            code.append("{Collector " + collectorVar + " = Collector.instance();");
            code.append(quietCode);
            code.append("}");
            m.insertAfter(code.toString(), false /* asFinally */);
        }
    }

    private String getTransactionStartCode (HitProbe p) {
        assert (p.getType() == ProbeType.TRANSACTION_START);

        StringBuilder code = new StringBuilder();
        code.append("{Collector c = Collector.instance();");
        code.append("c." + p.getType().getMethodName() + "(" + p.getId() + ");");
        code.append("c.hit(" + p.getId() + ");");
        code.append("}");
        return code.toString();
    }

    private String getTransactionEndCode (HitProbe p) {
        assert (p.getType() == ProbeType.TRANSACTION_END);

        StringBuilder code = new StringBuilder();
        code.append("{Collector c = Collector.instance();");
        code.append("c.hit(" + p.getId() + ");");
        code.append("c." + p.getType().getMethodName() + "(" + p.getId() + ");");
        code.append("}");
        return code.toString();
    }
}