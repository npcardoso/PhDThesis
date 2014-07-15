package io.crowbar.instrumentation;

import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;
import java.util.logging.Logger;
import java.util.*;
import java.io.FileOutputStream;

import javassist.*;
import javassist.bytecode.*;

public class Agent implements ClassFileTransformer {
    private static Logger log = Logger.getLogger("io.crowbar.instrumentation");


    public static void premain(String agentArgs, Instrumentation inst) {
        CtClass.debugDump = "debug";

        Agent a = new Agent();
        a.addPass(new InjectPass(InjectPass.Granularity.FUNCTION));
        a.addPass(new PrintByteCodePass());
//        a.addPass(new WrapJunit4());

        inst.addTransformer(a);
    }



    private List<Pass> passes = new LinkedList<Pass>();

    public boolean ignore(String cls) {
        String ignores[] = {"java/", "sun/","javassist/"};
        for(String s : ignores)
            if(cls.startsWith(s))
                return true;
        return false;
    }

    public void addPass(Pass p) {
        passes.add(p);
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
            cc = cp.makeClass(new java.io.ByteArrayInputStream(bytes));

            log.info("Transforming class: " + s);

            for (Pass p : passes) {
                currentPass = p.getClass().getName();
                p.transform(cc);
            }

            return cc.toBytecode();
        } catch (Exception ex) {
           log.warning("Pass '" + currentPass +
                       "' raised an exception:\n" + ex);
            ex.printStackTrace();
        }
        return null;
    }

    private void saveBytecode(String name,
                              byte[] bytecode){

        try{
            FileOutputStream out = new FileOutputStream("debug/" + name + ".class");
            out.write(bytecode);
            out.close();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }
}
