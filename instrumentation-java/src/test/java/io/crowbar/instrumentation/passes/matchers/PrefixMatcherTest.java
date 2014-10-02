package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;

import javassist.NotFoundException;

import org.junit.Test;

public class PrefixMatcherTest extends AbstractClassMatcherTest {
    public PrefixMatcherTest () throws NotFoundException {
        super();
    }

    @Test
    public void testSeveralMatchingPackages () {
        assertTrue(new PrefixMatcher("io").matches(testClass));
        assertTrue(new PrefixMatcher("io.crowbar").matches(testClass));
        assertTrue(new PrefixMatcher("io.crowbar.instrumentation").matches(testClass));
        assertTrue(new PrefixMatcher("").matches(testClass));
    }

    @Test
    public void testIncorrectPrefixes () {
        assertFalse(new PrefixMatcher("java").matches(testClass));
        assertFalse(new PrefixMatcher("foo").matches(testClass));
        assertFalse(new PrefixMatcher("io.foo").matches(testClass));
        assertFalse(new PrefixMatcher("io.crowbar.instrumentation.passes.matchers..").matches(testClass));
    }

    @Test
    public void testMultiplePrefixes () {
        assertTrue(new PrefixMatcher("io", "foo", "bar").matches(testClass));
        assertTrue(new PrefixMatcher("foo", "io", "bar").matches(testClass));
        assertTrue(new PrefixMatcher("foo", "bar", "io").matches(testClass));

        assertFalse(new PrefixMatcher("foo", "bar").matches(testClass));

        assertTrue(new PrefixMatcher(new ArrayList<String> () {
                                         private static final long serialVersionUID = 4950280704387196069L;
                                         {add("io");
                                          add("foo");
                                          add("bar");
                                         }
                                     }).matches(testClass));
        assertTrue(new PrefixMatcher(new ArrayList<String> () {
                                         private static final long serialVersionUID = -4649535971259526912L;
                                         {add("foo");
                                          add("io");
                                          add("bar");
                                         }
                                     }).matches(testClass));
        assertTrue(new PrefixMatcher(new ArrayList<String> () {
                                         private static final long serialVersionUID = 5611032468956303044L;
                                         {add("foo");
                                          add("bar");
                                          add("io");
                                         }
                                     }).matches(testClass));

        assertFalse(new PrefixMatcher(new ArrayList<String> () {
                                          private static final long serialVersionUID = -2700718722628812842L;
                                          {add("foo");
                                           add("bar");
                                          }
                                      }).matches(testClass));
    }

    @Test
    public void testMatchingMethod () {
        assertTrue(new PrefixMatcher("voidTest").matches(testClass, voidTestMethod));
        assertTrue(new PrefixMatcher("stringTestMethod").matches(testClass, stringTestMethod));
    }

    @Test
    public void testIncorrectMethodPrefixes () {
        assertFalse(new PrefixMatcher("foo").matches(testClass, voidTestMethod));
        assertFalse(new PrefixMatcher("bar").matches(testClass, voidTestMethod));
    }

    @Test
    public void testMultipleMethodPrefixes () {
        assertTrue(new PrefixMatcher("foo", "void").matches(testClass, voidTestMethod));
        assertTrue(new PrefixMatcher("string", "bar").matches(testClass, stringTestMethod));

        assertFalse(new PrefixMatcher("foo", "bar").matches(testClass, voidTestMethod));
    }
}