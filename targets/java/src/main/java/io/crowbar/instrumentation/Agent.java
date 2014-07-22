package io.crowbar.instrumentation;

import io.crowbar.instrumentation.events.VerboseListener;
import io.crowbar.instrumentation.events.MultiListener;
import io.crowbar.instrumentation.messaging.Client;
import io.crowbar.instrumentation.passes.IgnorePass;
import io.crowbar.instrumentation.passes.InjectPass;
import io.crowbar.instrumentation.passes.Pass;
import io.crowbar.instrumentation.passes.TestWrapperPass;
import io.crowbar.instrumentation.passes.wrappers.JUnit4Wrapper;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.ProbeSet;

import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.ProtectionDomain;
import java.util.LinkedList;
import java.util.List;

import javassist.ClassPool;
import javassist.CtClass;
import javassist.Modifier;

public class Agent implements ClassFileTransformer {
    public static void premain (String agentArgs, Instrumentation inst) {
        CtClass.debugDump = "debug";

        Agent a = new Agent();


        // Ignores classes in particular packages
        IgnorePass ip_black = new IgnorePass( /* blacklist */ true);
        ip_black.prefix.add("java.");
        ip_black.prefix.add("sun.");
        ip_black.prefix.add("javassist.");
        ip_black.prefix.add("org.junit.");
        ip_black.modifier_mask = Modifier.INTERFACE | Modifier.ANNOTATION;

        a.passes.add(ip_black);

        // IgnorePass ip_white = new IgnorePass(/* blacklist */ false);
        // ip_white.suffix.add("asda");
        // a.passes.add(ip_white);


        // Injects instrumentation instructions
        InjectPass inject = new InjectPass(InjectPass.Granularity.FUNCTION);
        a.passes.add(inject);


        // Wraps unit tests with instrumentation instrunctions
        TestWrapperPass twp = new TestWrapperPass();
        twp.wrappers.add(new JUnit4Wrapper());
        a.passes.add(twp);


        MultiListener ml = new MultiListener();
        ml.add(new VerboseListener());
        ml.add(new Client(null, Integer.parseInt(agentArgs)));
        Collector.getDefault().setListener(ml);


        inst.addTransformer(a);
    }

    @Override
    public final byte[] transform (ClassLoader classLoader,
                                   String s,
                                   Class< ? > aClass,
                                   ProtectionDomain protectionDomain,
                                   byte[] bytes) throws IllegalClassFormatException {
        CtClass c = null;
        ClassPool cp = null;


        try {
            cp = ClassPool.getDefault();
            c = cp.makeClass(new java.io.ByteArrayInputStream(bytes));
        }
        catch (Exception e) {
            e.printStackTrace();
            return bytes;
        }


        try {
            cp.importPackage("io.crowbar.instrumentation.runtime");

            for (Pass p : passes) {
                p.transform(c);
            }


            System.out.println("Instrumented Class: " + c.getName());
            return c.toBytecode();
        }
        catch (Pass.IgnoreClassException e) {
            // System.out.println("Ignoring Class: " + c.getName());
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }

        return bytes;
    }

    public List<Pass> passes = new LinkedList<Pass> ();
}