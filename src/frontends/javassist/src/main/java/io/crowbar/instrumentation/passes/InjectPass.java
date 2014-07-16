package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.*;

import javassist.*;
import javassist.bytecode.*;

public class InjectPass extends Pass {
    public InjectPass(Granularity g) {
        granularity = g;
    }

    @Override
    public void transform(CtClass c,
                          ProbeSet ps) throws Exception {

        for (CtMethod m : c.getDeclaredMethods()) {
            MethodInfo info = m.getMethodInfo();
            CodeAttribute ca = info.getCodeAttribute();

            if(ca == null)
                continue;

            handleMethod(ps, c, m, info, ca);
            ca.setMaxStack(ca.computeMaxStack());
        }

    }

    protected void handleMethod(ProbeSet ps,
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

            if(cur_line == -1)
                continue;
            if(cur_line <= last_line)
                continue;


            Bytecode b = getInstrumentationCodeFast(ps, c, m, cur_line, info.getConstPool());
            ci.insert(index, b.get());

            if(granularity == Granularity.FUNCTION)
                break;

        }
    }

    protected Bytecode getInstrumentationCode(ProbeSet ps,
                                              CtClass c,
                                              CtMethod m,
                                              int line,
                                              ConstPool p) {
        Bytecode b = new Bytecode(p);
        Collector collector = Collector.getDefault();
        int id = ps.register(ProbeType.HIT_PROBE,
                             c.getName(),
                             m.getName(),
                             line);

        System.out.println("Registered " + id + " = " + ps.get(id));
        b.addInvokestatic("io/crowbar/instrumentation/runtime/Collector",
                          "getDefault",
                          "()Lio/crowbar/instrumentation/runtime/Collector;");
        b.addIconst(id);
        b.addInvokevirtual("io/crowbar/instrumentation/runtime/Collector", "hitprobe", "(I)V");

        return b;
    }


    protected Bytecode getInstrumentationCodeFast(ProbeSet ps,
                                                  CtClass c,
                                                  CtMethod m,
                                                  int line,
                                                  ConstPool p) {
        Bytecode b = new Bytecode(p);
        Collector collector = Collector.getDefault();
        int id = ps.register(ProbeType.HIT_PROBE,
                             c.getName(),
                             m.getName(),
                             line);
        System.out.println("Registered " + id + " = " + ps.get(id));

        b.addGetstatic(c, "__CROWBAR_HIT_VECTOR__", "[Z");
        b.addIconst(id);
        b.addOpcode(Opcode.ICONST_1);
        b.addOpcode(Opcode.BASTORE);

//        mv.visitVarInsn(Opcodes.ALOAD, probeVariable);
//        mv.visitIntInsn(Opcodes.SIPUSH, id);
//        mv.visitInsn(Opcodes.ICONST_1);
//        mv.visitInsn(Opcodes.BASTORE);


        return b;
    }


    public enum Granularity {STATEMENT,
                             FUNCTION}
    private Granularity granularity;
}
