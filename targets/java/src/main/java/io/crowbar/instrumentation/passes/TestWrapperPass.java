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
                    m.insertBefore(getTransactionCode(c, n, ProbeType.TRANSACTION_START, false));
                    injectOracles(c, m, n, tw);
                    m.insertAfter(getTransactionCode(c, n, ProbeType.TRANSACTION_END, true),
                                  true /* asFinally */);
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
                                TestWrapper wrapper) throws Exception {
        final String exceptionVar = "eeeeeeeeeee";
        final String collectorVar = "ccccccccccc";
        HitProbe op = registerProbe(c, n, ProbeType.ORACLE);
        StringBuilder code = new StringBuilder();


        code.append("{Collector " + collectorVar + " = Collector.instance();");


        String oracleCode = wrapper.getOracleCode(c, m, n, op, collectorVar, exceptionVar);
        code.append((oracleCode != null) ? oracleCode : "");

        // TODO: Add reason to oracle
        code.append(collectorVar + ".hit(" + op.getId() + ");");
        code.append(collectorVar + "." + op.getType().getMethodName() + "(" + op.getId() + ", 1d, 1d);");
        code.append("throw " + exceptionVar + ";}");

        CtClass exceptionCls = ClassPool.getDefault().get("java.lang.Throwable");
        m.addCatch(code.toString(), exceptionCls, exceptionVar);
    }

    private String getTransactionCode (CtClass c,
                                       Node n,
                                       ProbeType type,
                                       boolean hitFirst) throws RegistrationException {
        HitProbe p = registerProbe(c, n, type);
        String ret = "Collector c = Collector.instance();";
        String hit = "c.hit(" + p.getId() + ");";


        if (hitFirst)
            ret += hit;

        ret += "c." + type.getMethodName() + "(";
        ret += p.getId();

        if (type == ProbeType.TRANSACTION_END)
            ret += ", null";

        ret += ");";

        if (!hitFirst)
            ret += hit;

        return ret;
    }
}