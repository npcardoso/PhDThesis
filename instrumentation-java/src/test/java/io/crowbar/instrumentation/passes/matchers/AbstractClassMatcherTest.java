package io.crowbar.instrumentation.passes.matchers;

import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;
import javassist.NotFoundException;

public class AbstractClassMatcherTest {
    protected final CtClass testClass;
    protected final CtMethod voidTestMethod;
    protected final CtMethod stringTestMethod;
    protected final CtMethod intTestMethod;
    protected final CtMethod doubleArrayTestMethod;
    protected final CtClass extendedTestClass;

    public static class TestClass {
        public void voidTestMethod () {}
        public String stringTestMethod () {return "";}
        public int intTestMethod () {return 0;}
        public Double[] doubleArrayTestMethod () {return new Double[1];}
    }

    public static class ExtendedTestClass extends TestClass {}

    public AbstractClassMatcherTest () throws NotFoundException {
        ClassPool cp = ClassPool.getDefault();


        testClass = cp.get(AbstractClassMatcherTest.class.getCanonicalName() + "$TestClass");
        voidTestMethod = testClass.getMethod("voidTestMethod", "()V");
        stringTestMethod = testClass.getMethod("stringTestMethod", "()Ljava/lang/String;");
        intTestMethod = testClass.getMethod("intTestMethod", "()I");
        doubleArrayTestMethod = testClass.getMethod("doubleArrayTestMethod", "()[Ljava/lang/Double;");

        extendedTestClass = cp.get(AbstractClassMatcherTest.class.getCanonicalName() + "$ExtendedTestClass");
    }
}