package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import javassist.NotFoundException;

import org.junit.Test;

public class AnnotationMatcherTest extends AbstractClassMatcherTest {
    public AnnotationMatcherTest () throws NotFoundException {
        super();
    }

    @Test
    public void testMatchingClass () {
        AnnotationMatcher matcher = new AnnotationMatcher(getAnnotationName());


        assertTrue(matcher.matches(annotationTestClass));
    }

    @Test
    public void testIncorrectAnnotations () {
        assertFalse(new AnnotationMatcher("DummyAnnotation").matches(annotationTestClass));
        assertFalse(new AnnotationMatcher("java.lang.Object").matches(annotationTestClass));
        assertFalse(new AnnotationMatcher("java.lang.Object").matches(testClass));
    }

    @Test
    public void testAnnotationMethods () {
        AnnotationMatcher matcher = new AnnotationMatcher(getAnnotationName());


        assertTrue(matcher.matches(annotationTestClass, annotationTestMethod));
        assertFalse(matcher.matches(annotationTestClass, noAnnotationTestMethod));
    }

    @Test
    public void testIncorrectMethodAnnotations () {
        assertFalse(new AnnotationMatcher("DummyAnnotation").matches(annotationTestClass, annotationTestMethod));
        assertFalse(new AnnotationMatcher("java.lang.Object").matches(annotationTestClass, annotationTestMethod));
        assertFalse(new AnnotationMatcher("java.lang.Object").matches(testClass, voidTestMethod));
    }
}