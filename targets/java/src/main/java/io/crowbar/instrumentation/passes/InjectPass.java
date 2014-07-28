package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import javassist.*;
import javassist.bytecode.*;

public class InjectPass extends AbstractPass {
    public enum Granularity {
        STATEMENT,
        FUNCTION
    }

    private static final String HIT_VECTOR_TYPE = "[Z";

    private final Granularity granularity;
    private final String hitVectorName;

    public InjectPass (Granularity granularity) {
        this(granularity, "__CROWBAR_HIT_VECTOR__");
    }

    public InjectPass (Granularity granularity,
                       String hitVectorName) {
        this.granularity = granularity;
        this.hitVectorName = hitVectorName;
    }

    @Override
    public Outcome transform (CtClass c) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            MethodInfo info = m.getMethodInfo();
            CodeAttribute ca = info.getCodeAttribute();

            if (ca == null)
                continue;

            handleMethod(c, m);
            ca.setMaxStack(ca.computeMaxStack());
        }

        if (Collector.getDefault().getHitVector().exists(c.getName())) {
            CtField f = CtField.make("public static boolean[]  " + hitVectorName + " = " +
                                     "Collector.getDefault().getHitVector().get(" +
                                     "\"" + c.getName() + "\");", c);
            c.addField(f);
        }

        return Outcome.CONTINUE;
    }

    protected void handleMethod (CtClass c,
                                 CtMethod m) throws Exception {
        MethodInfo info = m.getMethodInfo();
        CodeAttribute ca = info.getCodeAttribute();
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


            Bytecode b = getInstrumentationCode(c, m, cur_line, info.getConstPool());
            ci.insert(index, b.get());

            if (granularity == Granularity.FUNCTION)
                break;
        }
    }

    protected Bytecode getInstrumentationCode (CtClass c,
                                               CtMethod m,
                                               int line,
                                               ConstPool pool) throws RegistrationException {
        Bytecode b = new Bytecode(pool);
        Node n = getNode(c, m, line);
        Probe p = registerProbe(c, n, ProbeType.HIT_PROBE);


        b.addGetstatic(c, hitVectorName, HIT_VECTOR_TYPE);
        b.addIconst(p.getLocalId());
        b.addOpcode(Opcode.ICONST_1);
        b.addOpcode(Opcode.BASTORE);

        return b;
    }
}