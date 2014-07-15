package io.crowbar.instrumentation;

import io.crowbar.instrumentation.runtime.*;

import javassist.*;
import javassist.bytecode.*;

public class InjectStatementPass extends Pass {
    public enum Granularity {STATEMENT,
                             FUNCTION}


    private Granularity granularity;

    InjectStatementPass(Granularity g) {
        granularity = g;
    }

    @Override
    public void transform(ClassPool cp,
                          CtClass c) throws Exception {
//        for (CtClass c : cc.getDeclaredClasses()) {
//            log.info("Found class: " + c.getName());
//            transform(c);
//        }

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

    public void handleMethod(String classname,
                             String methodname,
                             MethodInfo info,
                             CodeAttribute ca) throws Exception {
        CodeIterator ci = ca.iterator();

        int last_line = -1;
        while (ci.hasNext()) {
            int index = ci.next();
            int cur_line = info.getLineNumber(index);

            if(cur_line == -1)
                continue;
            if(cur_line == last_line)
                continue;


            Bytecode b = getInstrumentationCode(classname,
                                                methodname,
                                                cur_line,
                                                info.getConstPool());
            ci.insert(index, b.get());

            last_line = cur_line;
        }

    }

    protected Bytecode getInstrumentationCode(String classname,
                                              String methodname,
                                              int line,
                                              ConstPool p) {
        Bytecode b = new Bytecode(p);
        Collector collector = Collector.getDefault();
        int id = collector.register(Collector.ProbeType.HIT_PROBE, classname, methodname, line);

        b.addInvokestatic("io/crowbar/instrumentation/runtime/Collector",
                          "getDefault",
                          "()Lio/crowbar/instrumentation/runtime/Collector;");
        b.addLconst(id);
        b.addInvokevirtual("io/crowbar/instrumentation/runtime/Collector", "hitprobe", "(J)V");

        return b;
    }
}
