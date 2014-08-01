package io.crowbar.instrumentation;

import io.crowbar.instrumentation.events.MultiListener;
import io.crowbar.instrumentation.events.VerboseListener;
import io.crowbar.instrumentation.messaging.Client;
import io.crowbar.instrumentation.passes.FilterPass;
import io.crowbar.instrumentation.passes.InjectPass;
import io.crowbar.instrumentation.passes.Pass;
import io.crowbar.instrumentation.passes.StackSizePass;
import io.crowbar.instrumentation.passes.TestWrapperPass;
import io.crowbar.instrumentation.passes.matchers.BlackList;
import io.crowbar.instrumentation.passes.matchers.ModifierMatcher;
import io.crowbar.instrumentation.passes.matchers.PrefixMatcher;
import io.crowbar.instrumentation.passes.wrappers.ActionTakerToTestWrapper;
import io.crowbar.instrumentation.passes.wrappers.TestNGTestWrapper;
import io.crowbar.instrumentation.passes.wrappers.JUnit3TestWrapper;
import io.crowbar.instrumentation.passes.wrappers.JUnit4TestWrapper;
import io.crowbar.instrumentation.runtime.Collector;

import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
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
        PrefixMatcher pMatcher = new PrefixMatcher("javax.",
                                                   "java.",
                                                   "sun.",
                                                   "javassist.",
                                                   "io.crowbar.instrumentation",
                                                   "org.apache.",
                                                   "org.junit.",
                                                   "org.eclipse.",
                                                   "com.sun.",
                                                   "junit.");

        ModifierMatcher mMatcher = new ModifierMatcher(Modifier.NATIVE);

        FilterPass fp = new FilterPass(new BlackList(mMatcher),
                                       new BlackList(pMatcher));
        a.passes.add(fp);


        // Injects instrumentation instructions
        InjectPass inject = new InjectPass(InjectPass.Granularity.FUNCTION);
        a.passes.add(inject);


        // Wraps unit tests with instrumentation instrunctions
        TestWrapperPass twp = new TestWrapperPass(
            new ActionTakerToTestWrapper(
                new BlackList(
                    new ModifierMatcher(Modifier.ABSTRACT))), // Skip Abstract Methods
            new JUnit3TestWrapper(),
            new JUnit4TestWrapper(),
            new TestNGTestWrapper());
        a.passes.add(twp);

        // Recalculates the stack size for all methods
        StackSizePass stackSizePass = new StackSizePass();
        a.passes.add(stackSizePass);


        MultiListener ml = new MultiListener();
        VerboseListener vl = new VerboseListener();
        vl.enableRegisterNode(false);
        vl.enableRegisterProbe(false);

        vl.setPrefix("!!!!!!!! ");
        // vl.setSuffix(" !!!!!!!!");

        Client cl = new Client(null, Integer.parseInt(agentArgs));
        ml.add(vl);
        ml.add(cl);

        Collector.instance().start("Workspace-" + cl.getCliendId(), ml);


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


        if (classLoader == null)
            return null;


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
                Pass.Outcome ret = p.transform(c);

                switch (ret) {
                case CONTINUE:
                    continue;

                case ABORT:
                    // System.err.println("Ignoring Class: " + c.getName());
                    return null;

                case RETURN:
                default:
                    break;
                }
            }


            // System.err.println("Instrumented Class: " + c.getName());
            return c.toBytecode();
        }
        catch (Exception ex) {
            System.err.println("Error in Class: " + c.getName());
            ex.printStackTrace();
        }

        return null;
    }

    private List<Pass> passes = new LinkedList<Pass> ();
}