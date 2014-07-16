package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.*;
import io.crowbar.instrumentation.passes.wrappers.*;

import javassist.*;
import javassist.bytecode.*;

import java.util.*;

public class TestWrapperPass extends Pass {

    @Override
    public void transform(CtClass c,
                          ProbeSet ps) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            for(Wrapper w : wrappers) {
                if (w.matches(m)) {
                    m.insertBefore(getInstrumentationCode(ProbeType.TRANSACTION_START,
                                                          c.getName(),
                                                          m.getName(),
                                                          ps));
                    m.insertAfter(getInstrumentationCode(ProbeType.TRANSACTION_END,
                                                         c.getName(),
                                                         m.getName(),
                                                         ps),
                                  true /* asFinally */);
                    break;
                }
            }
        }

    }
    protected String getInstrumentationCode(ProbeType type,
                                            String classname,
                                            String methodname,
                                            ProbeSet ps) {
        Collector collector = Collector.getDefault();
        int id = ps.register(type, classname, methodname);
        System.out.println("Registered " + id + " = " + ps.get(id));

        return "Collector.getDefault()." + type.method_name + "(" + id + ");";
  }

    public List<Wrapper> wrappers = new LinkedList<Wrapper>();
}
