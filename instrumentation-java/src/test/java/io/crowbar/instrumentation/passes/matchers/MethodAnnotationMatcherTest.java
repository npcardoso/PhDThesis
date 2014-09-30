package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import javassist.NotFoundException;

import org.junit.Test;

public class MethodAnnotationMatcherTest extends AbstractClassMatcherTest {
    public MethodAnnotationMatcherTest () throws NotFoundException {
        super();
    }

    @Test
    public void testMatchingClass () {
        MethodAnnotationMatcher matcher = new MethodAnnotationMatcher(getAnnotationName());


        assertFalse(matcher.matches(annotationTestClass));
    }

    @Test
    public void testIncorrectAnnotations () {
        assertFalse(new MethodAnnotationMatcher("DummyAnnotation").matches(annotationTestClass));
        assertFalse(new MethodAnnotationMatcher("java.lang.Object").matches(annotationTestClass));
        assertFalse(new MethodAnnotationMatcher("java.lang.Object").matches(testClass));
    }

    @Test
    public void testAnnotationMethods () {
        MethodAnnotationMatcher matcher = new MethodAnnotationMatcher(getAnnotationName());


        assertTrue(matcher.matches(annotationTestClass, annotationTestMethod));
        assertFalse(matcher.matches(annotationTestClass, noAnnotationTestMethod));
    }

    @Test
    public void testIncorrectMethodAnnotations () {
        assertFalse(new MethodAnnotationMatcher("DummyAnnotation").matches(annotationTestClass, annotationTestMethod));
        assertFalse(new MethodAnnotationMatcher("java.lang.Object").matches(annotationTestClass, annotationTestMethod));
        assertFalse(new MethodAnnotationMatcher("java.lang.Object").matches(testClass, voidTestMethod));
    }
}