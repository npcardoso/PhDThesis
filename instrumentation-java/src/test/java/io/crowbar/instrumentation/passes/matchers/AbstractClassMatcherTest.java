package io.crowbar.instrumentation.passes.matchers;

import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;
import javassist.NotFoundException;

public class AbstractClassMatcherTest {
    protected final CtClass testClass,
                            extendedTestClass,
                            annotationTestClass;

    protected final CtMethod voidTestMethod,
                             stringTestMethod,
                             intTestMethod,
                             doubleArrayTestMethod,
                             annotationTestMethod,
                             noAnnotationTestMethod;


    public static class TestClass {
        public void voidTestMethod () {}
        public String stringTestMethod () {return "";}
        public int intTestMethod () {return 0;}
        public Double[] doubleArrayTestMethod () {return new Double[1];}
    }

    public static class ExtendedTestClass extends TestClass {}

    public static @interface AnnotationClass {}

    protected String getAnnotationName () {
        return AbstractClassMatcherTest.class.getCanonicalName() + "$AnnotationClass";
    }

    @AnnotationClass
    public static class AnnotationTestClass {
        @AnnotationClass
        public void annotationTestMethod () {}
        public void noAnnotationTestMethod () {}
    }

    public AbstractClassMatcherTest () throws NotFoundException {
        ClassPool cp = ClassPool.getDefault();


        testClass = cp.get(AbstractClassMatcherTest.class.getCanonicalName() + "$TestClass");
        voidTestMethod = testClass.getMethod("voidTestMethod", "()V");
        stringTestMethod = testClass.getMethod("stringTestMethod", "()Ljava/lang/String;");
        intTestMethod = testClass.getMethod("intTestMethod", "()I");
        doubleArrayTestMethod = testClass.getMethod("doubleArrayTestMethod", "()[Ljava/lang/Double;");

        extendedTestClass = cp.get(AbstractClassMatcherTest.class.getCanonicalName() + "$ExtendedTestClass");

        annotationTestClass = cp.get(AbstractClassMatcherTest.class.getCanonicalName() + "$AnnotationTestClass");
        annotationTestMethod = annotationTestClass.getMethod("annotationTestMethod", "()V");
        noAnnotationTestMethod = annotationTestClass.getMethod("noAnnotationTestMethod", "()V");
    }
}