package io.crowbar.instrumentation;

import io.crowbar.instrumentation.runtime.*;

import javassist.*;
import javassist.bytecode.*;

public class InjectStatementPass extends Pass {
    @Override
    public void transform(ClassPool cp,
                          CtClass c) throws Exception {
//        for (CtClass c : cc.getDeclaredClasses()) {
//            log.info("Found class: " + c.getName());
//            transform(c);
//        }

        for (CtMethod m : c.getDeclaredMethods()) {
            log.info("Found Method: " + m.getName());


            MethodInfo info = m.getMethodInfo();
            CodeAttribute ca = info.getCodeAttribute();

            if(ca == null)
                continue;

            int old_stack_size = ca.getMaxStack();
            handleMethod(info, ca);
            ca.setMaxStack(ca.computeMaxStack());
        }
    }

    public void handleMethod(MethodInfo info,
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


            Bytecode b = new Bytecode(info.getConstPool());
            getInstrumentationCode(b,cur_line);
            ci.insert(index, b.get());

            last_line = cur_line;
        }

    }

    protected void getInstrumentationCode(Bytecode b,
                                          int line) {
        Collector collector = Collector.getDefault();
        long id = collector.registerProbe();
        System.out.println("getInstrumentationCode");
        // Remove this:
        id = 123;
        b.add(Opcode.NOP);
        b.addInvokestatic("io/crowbar/instrumentation/runtime/Collector",
                          "getDefault",
                          "()Lio/crowbar/instrumentation/runtime/Collector;");

        b.addLconst(id);

        b.addInvokevirtual("io/crowbar/instrumentation/runtime/Collector", "probe", "(J)V");
    }
}
