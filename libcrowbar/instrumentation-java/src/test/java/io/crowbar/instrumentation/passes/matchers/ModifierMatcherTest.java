package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;
import javassist.Modifier;
import javassist.NotFoundException;

import org.junit.Test;

public class ModifierMatcherTest {
    private final CtClass abstractClass;
    private final CtMethod abstractMethod;

    private final CtClass interfaceClass;
    private final CtMethod interfaceMethod;

    private final CtClass finalClass;
    private final CtMethod finalMethod;

    public static abstract class AbstractClass {
        public abstract void abstractMethod ();
    }

    public static interface InterfaceClass {
        void interfaceMethod ();
    }

    public static final class FinalClass {
        public final void finalMethod () {}
    }

    public ModifierMatcherTest () throws NotFoundException {
        ClassPool cp = ClassPool.getDefault();


        abstractClass = cp.get(this.getClass().getCanonicalName() + "$AbstractClass");
        abstractMethod = abstractClass.getMethod("abstractMethod", "()V");

        interfaceClass = cp.get(this.getClass().getCanonicalName() + "$InterfaceClass");
        interfaceMethod = interfaceClass.getMethod("interfaceMethod", "()V");

        finalClass = cp.get(this.getClass().getCanonicalName() + "$FinalClass");
        finalMethod = finalClass.getMethod("finalMethod", "()V");
    }

    @Test
    public void testAbstractClassModifiers () {
        ModifierMatcher matcher = new ModifierMatcher(Modifier.ABSTRACT);


        assertTrue(matcher.matches(abstractClass));
        assertTrue(matcher.matches(interfaceClass));
        assertFalse(matcher.matches(finalClass));
    }

    @Test
    public void testMultipleModifiers () {
        ModifierMatcher matcherMultipleModifier = new ModifierMatcher(Modifier.ABSTRACT | Modifier.FINAL);


        assertTrue(matcherMultipleModifier.matches(abstractClass));
        assertTrue(matcherMultipleModifier.matches(interfaceClass));
        assertTrue(matcherMultipleModifier.matches(finalClass));
    }

    @Test
    public void testInterfaceClassModifiers () {
        ModifierMatcher matcher = new ModifierMatcher(Modifier.INTERFACE);


        assertFalse(matcher.matches(abstractClass));
        assertTrue(matcher.matches(interfaceClass));
        assertFalse(matcher.matches(finalClass));
    }

    @Test
    public void testFinalClassModifiers () {
        ModifierMatcher matcher = new ModifierMatcher(Modifier.FINAL);


        assertFalse(matcher.matches(abstractClass));
        assertFalse(matcher.matches(interfaceClass));
        assertTrue(matcher.matches(finalClass));
    }

    @Test
    public void testAbstractMethodModifiers () {
        ModifierMatcher matcher = new ModifierMatcher(Modifier.ABSTRACT);


        assertTrue(matcher.matches(abstractClass, abstractMethod));
        assertTrue(matcher.matches(interfaceClass, interfaceMethod));
        assertFalse(matcher.matches(finalClass, finalMethod));
    }

    @Test
    public void testFinalMethodModifiers () {
        ModifierMatcher matcher = new ModifierMatcher(Modifier.FINAL);


        assertFalse(matcher.matches(abstractClass, abstractMethod));
        assertFalse(matcher.matches(interfaceClass, interfaceMethod));
        assertTrue(matcher.matches(finalClass, finalMethod));
    }
}