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

    @Test
    public void testIncorrectReturnTypes () {
        assertFalse(new ReturnTypeMatcher("").matches(testClass, voidTestMethod));
        assertFalse(new ReturnTypeMatcher("String").matches(testClass, stringTestMethod));
        assertFalse(new ReturnTypeMatcher("long").matches(testClass, intTestMethod));
        assertFalse(new ReturnTypeMatcher("double[]").matches(testClass, doubleArrayTestMethod));
    }
}