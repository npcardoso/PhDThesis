package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import javassist.CtClass;
import javassist.CtMethod;
import javassist.CtField;
import javassist.bytecode.MethodInfo;
import javassist.bytecode.CodeAttribute;
import javassist.bytecode.CodeIterator;
import javassist.bytecode.Bytecode;
import javassist.bytecode.ConstPool;
import javassist.bytecode.Opcode;

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
    public final Outcome transform (CtClass c) throws Exception {
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

    private void handleMethod (CtClass c,
                               CtMethod m) throws Exception {
        MethodInfo info = m.getMethodInfo();
        CodeAttribute ca = info.getCodeAttribute();
        CodeIterator ci = ca.iterator();


        for (int lastLine = -1, index, curLine;
             ci.hasNext();
             lastLine = curLine) {
            index = ci.next();
            curLine = info.getLineNumber(index);

            if (curLine == -1)
                continue;

            if (curLine <= lastLine)
                continue;


            Bytecode b = getInstrumentationCode(c, m, curLine, info.getConstPool());
            ci.insert(index, b.get());

            if (granularity == Granularity.FUNCTION)
                break;
        }
    }

    private Bytecode getInstrumentationCode (CtClass c,
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