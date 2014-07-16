package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.*;
import io.crowbar.instrumentation.passes.wrappers.*;

import javassist.*;
import javassist.bytecode.*;

import java.util.*;

public class TestWrapperPass extends Pass {

    @Override
    public void transform(CtClass c) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            for(Wrapper w : wrappers) {
                if (w.matches(m)) {
                    m.insertBefore(getInstrumentationCode(Collector.ProbeType.TRANSACTION_START,
                                                          c.getName(),
                                                          m.getName()));
                    m.insertAfter(getInstrumentationCode(Collector.ProbeType.TRANSACTION_END,
                                                         c.getName(),
                                                         m.getName()),
                                  true /* asFinally */);
                    break;
                }
            }
        }

    }
    protected String getInstrumentationCode(Collector.ProbeType type,
                                            String classname,
                                            String methodname) {
        Collector collector = Collector.getDefault();
        int id = collector.register(type, classname, methodname);
        System.out.println("Registered " + id + " = " + collector.getProbe(id));

        return "Collector.getDefault()." + type.method_name + "(" + id + ");";
  }

    public List<Wrapper> wrappers = new LinkedList<Wrapper>();
}
