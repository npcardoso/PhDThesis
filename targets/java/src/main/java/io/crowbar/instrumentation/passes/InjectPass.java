package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.ProbeGroup.Probe;
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
        boolean injected = false;


        for (CtMethod m : c.getDeclaredMethods()) {
            MethodInfo info = m.getMethodInfo();
            CodeAttribute ca = info.getCodeAttribute();

            if (ca == null)
                continue;

            injected = injected || handleMethod(c, m);
            ca.setMaxStack(ca.computeMaxStack());
        }

        if (injected) {
            CtField f = CtField.make("public static boolean[]  " + hitVectorName + " = " +
                                     "Collector.instance().getHitVector(" +
                                     "\"" + c.getName() + "\");", c);
            c.addField(f);
        }

        return Outcome.CONTINUE;
    }

    /*!
     * \brief Handles instrumentation injection on a method level
     * @return Returns false if no probe was injected, true otherwise.
     */
    private boolean handleMethod (CtClass c,
                                  CtMethod m) throws Exception {
        MethodInfo info = m.getMethodInfo();
        CodeAttribute ca = info.getCodeAttribute();
        CodeIterator ci = ca.iterator();
        boolean injected = false;


        for (int lastLine = -1, index, curLine;
             ci.hasNext();
             lastLine = curLine) {
            index = ci.next();
            curLine = info.getLineNumber(index);

            if (curLine == -1)
                continue;

            if (curLine <= lastLine)
                continue;


            Node n = (granularity == Granularity.FUNCTION) ?
                     getNode(c, m) : getNode(c, m, curLine);

            Bytecode b = getInstrumentationCode(c, n, info.getConstPool());
            ci.insert(index, b.get());
            injected = true;

            if (granularity == Granularity.FUNCTION)
                break;
        }

        return injected;
    }

    private Bytecode getInstrumentationCode (CtClass c,
                                             Node n,
                                             ConstPool pool) throws RegistrationException {
        Bytecode b = new Bytecode(pool);
        Probe p = registerProbe(c, n, ProbeType.HIT_PROBE);


        b.addGetstatic(c, hitVectorName, HIT_VECTOR_TYPE);
        b.addIconst(p.getLocalId());
        b.addOpcode(Opcode.ICONST_1);
        b.addOpcode(Opcode.BASTORE);

        return b;
    }
}