package io.crowbar.instrumentation;

import io.crowbar.instrumentation.passes.*;
import io.crowbar.instrumentation.passes.wrappers.*;
import io.crowbar.instrumentation.runtime.*;


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


        // Ignores classes in particular packages
        IgnorePass ip_black = new IgnorePass(/* blacklist */ true);
        ip_black.prefix.add("java.");
        ip_black.prefix.add("sun.");
        ip_black.prefix.add("javassist.");
        ip_black.prefix.add("org.junit.");
        ip_black.modifier_mask = Modifier.INTERFACE | Modifier.ANNOTATION;

        a.passes.add(ip_black);

        //IgnorePass ip_white = new IgnorePass(/* blacklist */ false);
        //ip_white.suffix.add("asda");
        //a.passes.add(ip_white);

        // Wraps unit tests with instrumentation instrunctions
        TestWrapperPass twp = new TestWrapperPass();
        twp.wrappers.add(new JUnit4Wrapper());
        a.passes.add(twp);

        // Injects instrumentation instructions
        InjectPass inject = new InjectPass(InjectPass.Granularity.FUNCTION);
        a.passes.add(inject);


        inst.addTransformer(a);
    }

    @Override
    public final byte[] transform(ClassLoader classLoader,
                                  String s,
                                  Class<?> aClass,
                                  ProtectionDomain protectionDomain,
                                  byte[] bytes) throws IllegalClassFormatException {
        CtClass c = null;
        ClassPool cp = ClassPool.getDefault();

        try {
            c = cp.makeClass(new java.io.ByteArrayInputStream(bytes));
        }
        catch (Exception e) {
            e.printStackTrace();
            return bytes;
        }


        try {

            cp.importPackage("io.crowbar.instrumentation.runtime");

            ProbeSet ps = new ProbeSet(c.getName());
            for (Pass p : passes) {
                p.transform(c, ps);
            }
            Collector.getDefault().register(ps);

            System.out.println("Instrumented Class: " + c.getName());
            return c.toBytecode();
        }
        catch (Pass.IgnoreClassException e) {
//            System.out.println("Ignoring Class: " + c.getName());
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }

        return bytes;
    }

    public List<Pass> passes = new LinkedList<Pass>();
}
