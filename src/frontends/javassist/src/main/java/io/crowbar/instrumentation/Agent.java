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

        // Wraps unit tests with instrumentation instrunctions
        TestWrapperPass twp = new TestWrapperPass();
        twp.wrappers.add(new JUnit4Wrapper());
        a.passes.add(twp);


        // Ignores classes in particular packages
        IgnorePass ip_black = new IgnorePass(/* blacklist */ true);
        ip_black.prefix.add("java.");
        ip_black.prefix.add("sun.");
        ip_black.prefix.add("javassist.");
        ip_black.prefix.add("org.junit.");
        a.passes.add(ip_black);

        // Ignores classes in particular packages
        //IgnorePass ip_white = new IgnorePass(/* blacklist */ false);
        //ip_white.suffix.add("asda");
        //a.passes.add(ip_white);


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
        }
        catch (Pass.IgnoreClassException e) {
            System.out.println("Ignoring Class: " + s);
        }
        catch (Exception ex) {
            System.err.println("Pass '" + currentPass +
                               "' raised an exception:\n" + ex);
            ex.printStackTrace();
            return null;
        }

        try {
            return cc.toBytecode();
        } catch (Exception ex) {
            return null;
        }
    }

    public List<Pass> passes = new LinkedList<Pass>();
}
