package io.crowbar.instrumentation;

import io.crowbar.instrumentation.passes.Pass;

import java.io.IOException;
import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;

import javassist.ClassPool;
import javassist.CtClass;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;


public class Agent implements ClassFileTransformer {
    private static final Logger logger = LogManager.getLogger(Agent.class);

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
        logger.info("----------- Crowbar Agent Ready -----------");
        logger.info("----------- Agent Configs -----------");
        logger.info(a.agentConfigs.serialize());
        logger.info("----------- Agent Configs -----------");
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


        byte[] ret = null;

        if (classLoader == null)
            return null;


        try {
            cp = ClassPool.getDefault();
            c = cp.makeClass(new java.io.ByteArrayInputStream(bytes));
        }
        catch (IOException e) {
            logger.error(e, e);
            return null;
        }
        catch (RuntimeException e) {
            logger.error(e, e);
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
                    logger.debug("Ignoring Class: {}", c.getName());
                    return null;

                case RETURN:
                default:
                    break;
                }
            }
            
            ret = c.toBytecode();
            logger.debug("Instrumented Class: {}", c.getName());
        }
        catch (Throwable ex) {
            logger.error("Error in Class: {}", c.getName());
            logger.error(ex, ex);
        }

        c.detach();
        return ret;
    }

    private AgentConfigs agentConfigs = null;
}