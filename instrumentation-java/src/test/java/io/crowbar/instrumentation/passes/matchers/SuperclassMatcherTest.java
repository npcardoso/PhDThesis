package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import javassist.NotFoundException;

import org.junit.Test;

public class SuperclassMatcherTest extends AbstractClassMatcherTest {
    public SuperclassMatcherTest () throws NotFoundException {
        super();
    }

    @Test
    public void testMatchingSuperClass () {
        assertTrue(new SuperclassMatcher("java.lang.Object").matches(testClass));

        String testClassName = AbstractClassMatcherTest.class.getCanonicalName() + "$TestClass";
        assertTrue(new SuperclassMatcher(testClassName).matches(extendedTestClass));
    }

    @Test
    public void testTransitiveSuperClass () {
        assertTrue(new SuperclassMatcher("java.lang.Object").matches(extendedTestClass));
    }

    @Test
    public void testIncorrectSuperClass () {
        assertFalse(new SuperclassMatcher("java.lang.String").matches(testClass));
        assertFalse(new SuperclassMatcher("").matches(extendedTestClass));
    }
}