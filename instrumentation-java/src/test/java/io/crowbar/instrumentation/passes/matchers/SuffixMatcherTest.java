package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import javassist.NotFoundException;

import org.junit.Test;

public class SuffixMatcherTest extends AbstractClassMatcherTest {
    public SuffixMatcherTest () throws NotFoundException {
        super();
    }

    @Test
    public void testSeveralMatchingClassSuffixes () {
        assertTrue(new SuffixMatcher("Class").matches(testClass));
        assertTrue(new SuffixMatcher("TestClass").matches(testClass));
        assertTrue(new SuffixMatcher("AbstractClassMatcherTest$TestClass").matches(testClass));
        assertTrue(new SuffixMatcher("").matches(testClass));
    }

    @Test
    public void testIncorrectClassSuffixes () {
        assertFalse(new SuffixMatcher("java").matches(testClass));
        assertFalse(new SuffixMatcher("foo").matches(testClass));
        assertFalse(new SuffixMatcher("io.foo").matches(testClass));
        assertFalse(new SuffixMatcher("AbstractStringMatcherTest.TestClass").matches(testClass));
    }

    @Test
    public void testMultipleSuffixes () {
        assertTrue(new SuffixMatcher("TestClass", "foo", "bar").matches(testClass));
        assertTrue(new SuffixMatcher("foo", "TestClass", "bar").matches(testClass));
        assertTrue(new SuffixMatcher("foo", "bar", "TestClass").matches(testClass));

        assertFalse(new SuffixMatcher("foo", "bar").matches(testClass));
    }

    @Test
    public void testMatchingMethod () {
        assertTrue(new SuffixMatcher("Method").matches(testClass, voidTestMethod));
        assertTrue(new SuffixMatcher("stringTestMethod").matches(testClass, stringTestMethod));
    }

    @Test
    public void testIncorrectMethodSuffixes () {
        assertFalse(new SuffixMatcher("foo").matches(testClass, voidTestMethod));
        assertFalse(new SuffixMatcher("bar").matches(testClass, voidTestMethod));
    }

    @Test
    public void testMultipleMethodSuffixes () {
        assertTrue(new SuffixMatcher("foo", "Method").matches(testClass, voidTestMethod));
        assertTrue(new SuffixMatcher("Method", "bar").matches(testClass, stringTestMethod));

        assertFalse(new SuffixMatcher("foo", "bar").matches(testClass, voidTestMethod));
    }
}