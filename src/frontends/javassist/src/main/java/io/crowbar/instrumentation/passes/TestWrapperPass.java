package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.*;
import io.crowbar.instrumentation.passes.wrappers.*;

import javassist.*;
import javassist.bytecode.*;

import java.util.*;

public class TestWrapperPass extends Pass {
    @Override
    public void transform (CtClass c,
                           ProbeSet ps) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            for (Wrapper w : wrappers) {
                if (w.matches(m)) {
                    m.insertBefore(getInstrumentationCode(ProbeType.TRANSACTION_START,
                                                          m.getName(),
                                                          ps));
                    m.insertAfter(getInstrumentationCode(ProbeType.TRANSACTION_END,
                                                         m.getName(),
                                                         ps),
                                  true /* asFinally */);
                    break;
                }
            }
        }
    }

    protected String getInstrumentationCode (ProbeType type,
                                             String methodname,
                                             ProbeSet ps) throws ProbeSet.AlreadyPreparedException {
        int id = ps.register(type, methodname);


        return "Collector.getDefault()." + type.method_name + "(\"" + ps.getClassName() + "\", " + id + ");";
    }

    public List<Wrapper> wrappers = new LinkedList<Wrapper> ();
}