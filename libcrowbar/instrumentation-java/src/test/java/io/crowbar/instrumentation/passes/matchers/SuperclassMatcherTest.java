package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import javassist.NotFoundException;

import org.junit.Test;

public class SuperclassMatcherTest extends AbstractClassMatcherTest {
    public SuperclassMatcherTest () throws NotFoundException {
        super();
    }

    @Test
    public void testMatchingSuperclass () {
        assertTrue(new SuperclassMatcher("java.lang.Object").matches(testClass));

        String testClassName = AbstractClassMatcherTest.class.getCanonicalName() + "$TestClass";
        assertTrue(new SuperclassMatcher(testClassName).matches(extendedTestClass));
    }

    @Test
    public void testTransitiveSuperclass () {
        assertTrue(new SuperclassMatcher("java.lang.Object").matches(extendedTestClass));
    }

    @Test
    public void testIncorrectSuperclass () {
        assertFalse(new SuperclassMatcher("java.lang.String").matches(testClass));
        assertFalse(new SuperclassMatcher("").matches(extendedTestClass));
    }

    @Test
    public void tryToMatchMethods () {
        assertTrue(new SuperclassMatcher("java.lang.Object").matches(testClass, voidTestMethod));
        assertTrue(new SuperclassMatcher("java.lang.Object").matches(extendedTestClass, intTestMethod));
        assertFalse(new SuperclassMatcher("java.lang.String").matches(testClass, doubleArrayTestMethod));
    }
}