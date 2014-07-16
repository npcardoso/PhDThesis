package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.*;

import javassist.*;
import javassist.bytecode.*;

public class InjectPass extends Pass {
    public InjectPass(Granularity g) {
        granularity = g;
    }

    @Override
    public void transform(CtClass c) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            MethodInfo info = m.getMethodInfo();
            CodeAttribute ca = info.getCodeAttribute();

            if(ca == null)
                continue;

            handleMethod(c.getName(),
                         m.getName(),
                         info,
                         ca);
            ca.setMaxStack(ca.computeMaxStack());
        }
    }

    protected void handleMethod(String classname,
                                String methodname,
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


            Bytecode b = getInstrumentationCode(classname,
                                                methodname,
                                                cur_line,
                                                info.getConstPool());
            ci.insert(index, b.get());

            if(granularity == Granularity.FUNCTION)
                break;

        }
    }

    protected Bytecode getInstrumentationCode(String classname,
                                              String methodname,
                                              int line,
                                              ConstPool p) {
        Bytecode b = new Bytecode(p);
        Collector collector = Collector.getDefault();
        int id = collector.register(Collector.ProbeType.HIT_PROBE,
                                    classname,
                                    methodname,
                                    line);
        System.out.println("Registered " + id + " = " + collector.getProbe(id));
        b.addInvokestatic("io/crowbar/instrumentation/runtime/Collector",
                          "getDefault",
                          "()Lio/crowbar/instrumentation/runtime/Collector;");
        b.addIconst(id);
        b.addInvokevirtual("io/crowbar/instrumentation/runtime/Collector", "hitprobe", "(I)V");

        return b;
    }



    public enum Granularity {STATEMENT,
                             FUNCTION}
    private Granularity granularity;
}
