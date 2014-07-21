package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.*;
import io.crowbar.instrumentation.runtime.ProbeSet.AlreadyPreparedException;
import javassist.*;
import javassist.bytecode.*;

public class InjectPass extends Pass {
    public InjectPass (Granularity g) {
        granularity = g;
    }

    @Override
    public void transform (CtClass c,
                           ProbeSet ps) throws Exception {
        boolean injected = false;


        for (CtMethod m : c.getDeclaredMethods()) {
            MethodInfo info = m.getMethodInfo();
            CodeAttribute ca = info.getCodeAttribute();

            if (ca == null)
                continue;

            injected = true;
            handleMethod(ps, c, m, info, ca);
            ca.setMaxStack(ca.computeMaxStack());
        }

        if (injected) {
            CtField f = CtField.make("public static boolean[]  " + HIT_VECTOR_NAME + " = " +
                                     "Collector.getDefault().getHitVector(\"" + ps.getName() +
                                     "\");", c);
            c.addField(f);
        }
    }

    protected void handleMethod (ProbeSet ps,
                                 CtClass c,
                                 CtMethod m,
                                 MethodInfo info,
                                 CodeAttribute ca) throws Exception {
        CodeIterator ci = ca.iterator();


        for (int last_line = -1, index, cur_line;
             ci.hasNext();
             last_line = cur_line) {
            index = ci.next();
            cur_line = info.getLineNumber(index);

            if (cur_line == -1)
                continue;

            if (cur_line <= last_line)
                continue;


            Bytecode b = getInstrumentationCode(ps, c, m, cur_line, info.getConstPool());
            ci.insert(index, b.get());

            if (granularity == Granularity.FUNCTION)
                break;
        }
    }

    protected Bytecode getInstrumentationCode (ProbeSet ps,
                                               CtClass c,
                                               CtMethod m,
                                               int line,
                                               ConstPool p) throws AlreadyPreparedException {
        Bytecode b = new Bytecode(p);
        Collector collector = Collector.getDefault();
        int id = ps.register(ProbeType.HIT_PROBE,
                             m.getName(),
                             line);


        b.addGetstatic(c, HIT_VECTOR_NAME, HIT_VECTOR_TYPE);
        b.addIconst(id);
        b.addOpcode(Opcode.ICONST_1);
        b.addOpcode(Opcode.BASTORE);

        return b;
    }

    private final String HIT_VECTOR_NAME = "__CROWBAR_HIT_VECTOR__";
    private final String HIT_VECTOR_TYPE = "[Z";

    public enum Granularity {
        STATEMENT,
        FUNCTION
    }
    private Granularity granularity;
}