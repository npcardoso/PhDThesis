package io.crowbar.instrumentation.agents;

import java.security.*;
import java.lang.instrument.*;
import java.util.*;
import javassist.*;

public class TestAgent2 implements ClassFileTransformer {

  public TestAgent2() {
    super();
  }

  public byte[] transform(ClassLoader loader, String className, Class redefiningClass, ProtectionDomain domain, byte[] bytes) throws IllegalClassFormatException {
      System.out.println("Transforming: " + className);
      return transformClass(redefiningClass,bytes);
  }

  private byte[] transformClass(Class classToTransform, byte[] b) {
    ClassPool pool = ClassPool.getDefault();
    CtClass cl = null;
	try {
      cl = pool.makeClass(new java.io.ByteArrayInputStream(b));
      CtBehavior[] methods = cl.getDeclaredBehaviors();
      for (int i = 0; i < methods.length; i++) {
        if (methods[i].isEmpty() == false) {
          changeMethod(methods[i]);
        }
      }
      b = cl.toBytecode();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
    finally {
      if (cl != null) {
        cl.detach();
      }
    }
    return b;
  }

  private void changeMethod(CtBehavior method) throws NotFoundException, CannotCompileException {
      method.insertBefore("System.out.println(\"started method at \" + new java.util.Date());");
      method.insertAfter("System.out.println(\"ended method at \" + new java.util.Date());");
  }
}
