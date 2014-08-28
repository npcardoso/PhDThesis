package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;


public class AnnotationMatcher implements Matcher {
    private final String annotation;

    public AnnotationMatcher (String annotation) {
        this.annotation = annotation;
    }

    @Override
    public final boolean matches (CtClass c) {
        try {
            return c.hasAnnotation(Class.forName(annotation));
        }
        catch (Exception e) {
            return false;
        }
    }

    @Override
    public final boolean matches (CtClass c,
                                  CtMethod m) {
        try {
            return m.hasAnnotation(Class.forName(annotation));
        }
        catch (Exception e) {
            return false;
        }
    }
}