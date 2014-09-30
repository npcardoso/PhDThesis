package io.crowbar.instrumentation;

import io.crowbar.instrumentation.passes.Pass;

import java.io.IOException;
import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;

import javassist.ClassPool;
import javassist.CtClass;

public class Agent implements ClassFileTransformer {
    public static void premain (String agentArgs,
                                Instrumentation inst) {
        Agent a = new Agent();


        if (agentArgs != null && agentArgs.length() >= 0) {
            a.agentConfigs = AgentConfigs.deserialize(agentArgs);
        }

        if (a.agentConfigs == null) {
            a.agentConfigs = new AgentConfigs();
        }

        a.agentConfigs.configure();
        inst.addTransformer(a);
        System.err.println("----------- Crowbar Agent Ready -----------");
    }

    @Override
    public final byte[] transform (ClassLoader classLoader,
                                   String s,
                                   Class< ? > aClass,
                                   ProtectionDomain protectionDomain,
                                   byte[] bytes) throws IllegalClassFormatException {
        CtClass c = null;
        ClassPool cp = null;


        byte[] ret = null;

        if (classLoader == null)
            return null;


        try {
            cp = ClassPool.getDefault();
            c = cp.makeClass(new java.io.ByteArrayInputStream(bytes));
        }
        catch (IOException e) {
            return null;
        }
        catch (RuntimeException e) {
            return null;
        }


        try {
            cp.importPackage("io.crowbar.instrumentation.runtime");

            for (Pass p : agentConfigs.getPasses()) {
                switch (p.transform(c)) {
                case CONTINUE:
                    continue;

                case ABORT:
                    c.detach();
                    // System.err.println("Ignoring Class: " + c.getName());
                    return null;

                case RETURN:
                default:
                    break;
                }
            }

            ret = c.toBytecode();

            // System.err.println("Instrumented Class: " + c.getName());
        }
        catch (Throwable ex) {
            System.err.println("Error in Class: " + c.getName());
            ex.printStackTrace();
        }

        c.detach();
        return ret;
    }

    private AgentConfigs agentConfigs = null;
}