package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import javassist.NotFoundException;

import org.junit.Test;

public class ClassAnnotationMatcherTest extends AbstractClassMatcherTest {
    public ClassAnnotationMatcherTest () throws NotFoundException {
        super();
    }

    @Test
    public void testMatchingClass () {
        ClassAnnotationMatcher matcher = new ClassAnnotationMatcher(getAnnotationName());


        assertTrue(matcher.matches(annotationTestClass));
    }

    @Test
    public void testIncorrectAnnotations () {
        assertFalse(new ClassAnnotationMatcher("DummyAnnotation").matches(annotationTestClass));
        assertFalse(new ClassAnnotationMatcher("java.lang.Object").matches(annotationTestClass));
        assertFalse(new ClassAnnotationMatcher("java.lang.Object").matches(testClass));
    }

    @Test
    public void testAnnotationMethods () {
        ClassAnnotationMatcher matcher = new ClassAnnotationMatcher(getAnnotationName());


        assertTrue(matcher.matches(annotationTestClass, annotationTestMethod));
        assertTrue(matcher.matches(annotationTestClass, noAnnotationTestMethod));
    }

    @Test
    public void testIncorrectMethodAnnotations () {
        assertFalse(new ClassAnnotationMatcher("DummyAnnotation").matches(annotationTestClass, annotationTestMethod));
        assertFalse(new ClassAnnotationMatcher("java.lang.Object").matches(annotationTestClass, annotationTestMethod));
        assertFalse(new ClassAnnotationMatcher("java.lang.Object").matches(testClass, voidTestMethod));
    }
}