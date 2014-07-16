package io.crowbar.instrumentation;

import io.crowbar.instrumentation.passes.*;
import io.crowbar.instrumentation.passes.wrappers.*;


import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;
import java.util.*;
import java.io.FileOutputStream;

import javassist.*;
import javassist.bytecode.*;

public class Agent implements ClassFileTransformer {
    public static void premain(String agentArgs, Instrumentation inst) {
        CtClass.debugDump = "debug";

        Agent a = new Agent();
        a.passes.add(new InjectPass(InjectPass.Granularity.FUNCTION));

        TestWrapperPass twp = new TestWrapperPass();
        twp.wrappers.add(new JUnit4Wrapper());
        a.passes.add(twp);

        inst.addTransformer(a);
    }


    public boolean ignore(String cls) {
        String ignores[] = {"java/", "sun/","javassist/"};
        for(String s : ignores)
            if(cls.startsWith(s))
                return true;
        return false;
    }


    @Override
    public final byte[] transform(ClassLoader classLoader,
                                  String s,
                                  Class<?> aClass,
                                  ProtectionDomain protectionDomain,
                                  byte[] bytes) throws IllegalClassFormatException {
        MethodInfo.doPreverify = true;
        if(ignore(s)) {
            return bytes;
        }

        String currentPass = null;
        CtClass cc = null;
       try {
            ClassPool cp = ClassPool.getDefault();
            cp.importPackage("io.crowbar.instrumentation.runtime");

            cc = cp.makeClass(new java.io.ByteArrayInputStream(bytes));

            for (Pass p : passes) {
                currentPass = p.getClass().getName();
                p.transform(cc);
            }

            return cc.toBytecode();
        } catch (Exception ex) {
           System.err.println("Pass '" + currentPass +
                              "' raised an exception:\n" + ex);
           ex.printStackTrace();
        }
        return null;
    }

    public List<Pass> passes = new LinkedList<Pass>();
}
