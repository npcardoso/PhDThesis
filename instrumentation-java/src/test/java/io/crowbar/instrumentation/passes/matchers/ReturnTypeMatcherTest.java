package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import javassist.NotFoundException;

import org.junit.Test;

public class ReturnTypeMatcherTest extends AbstractClassMatcherTest {
    public ReturnTypeMatcherTest () throws NotFoundException {
        super();
    }

    @Test
    public void testMatchingReturnTypes () {
        assertTrue(new ReturnTypeMatcher("void").matches(testClass, voidTestMethod));
        assertTrue(new ReturnTypeMatcher("java.lang.String").matches(testClass, stringTestMethod));
        assertTrue(new ReturnTypeMatcher("int").matches(testClass, intTestMethod));
        assertTrue(new ReturnTypeMatcher("java.lang.Double[]").matches(testClass, doubleArrayTestMethod));
    }

    /*
     * To reach the catch branch, we would need to create a method with an unresolved return type
     */
    @Test
    public void testIncorrectReturnTypes () {
        assertFalse(new ReturnTypeMatcher("").matches(testClass, voidTestMethod));
        assertFalse(new ReturnTypeMatcher("String").matches(testClass, stringTestMethod));
        assertFalse(new ReturnTypeMatcher("long").matches(testClass, intTestMethod));
        assertFalse(new ReturnTypeMatcher("double[]").matches(testClass, doubleArrayTestMethod));
    }

    @Test
    public void tryToMatchCtClass () {
        // should always return false since there is no return method in CtClasses
        assertFalse(new ReturnTypeMatcher("void").matches(testClass));
        assertFalse(new ReturnTypeMatcher(testClass.getName()).matches(testClass));
    }
}