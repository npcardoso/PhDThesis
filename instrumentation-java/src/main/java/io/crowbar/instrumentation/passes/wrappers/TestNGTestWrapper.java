package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AndMatcher;
import io.crowbar.instrumentation.passes.matchers.AnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.ReturnTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;
import io.crowbar.instrumentation.runtime.Collector;

import java.lang.reflect.Method;
import java.util.regex.Pattern;

import javassist.CtClass;
import javassist.CtMethod;
import java.lang.annotation.Annotation;


public class TestNGTestWrapper extends AbstractTestWrapper {
    private static final String ANNOTATION_CLASS = "org.testng.annotations.Test";
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
            new AndMatcher(
                new AnnotationMatcher(ANNOTATION_CLASS),
                new ReturnTypeMatcher("void")));

    public static final boolean isPass (Class cls,
                                        String methodName) {
        try {
            Method method = cls.getMethod(methodName);
            Annotation annotation = method.getAnnotation((Class< ? extends Annotation> )Class.forName(ANNOTATION_CLASS));
            method = annotation.getClass().getMethod("expectedExceptions");
            Class< ? >[] expected = (Class[])method.invoke(annotation);
            method = annotation.getClass().getMethod("expectedExceptionsMessageRegExp");
            String expectedMsgRegex = (String) method.invoke(annotation);
            return expected.length == 0 && expectedMsgRegex.length() == 0;
        }
        catch (Throwable e) {}
        return true;
    }

    public static final boolean isPass (Class cls,
                                        String methodName,
                                        Throwable e) {
        try {
            Method method = cls.getMethod(methodName);
            Object annotation = method.getAnnotation((Class< ? extends Annotation> )Class.forName(ANNOTATION_CLASS));
            method = annotation.getClass().getMethod("expectedExceptions");
            Class< ? >[] expected = (Class[])method.invoke(annotation);
            method = annotation.getClass().getMethod("expectedExceptionsMessageRegExp");
            String expectedMsgRegex = (String) method.invoke(annotation);

            for (Class c : expected) {
                if (isSameType(e, c) &&
                    Pattern.matches(expectedMsgRegex, e.getMessage()))
                    return true;
            }
        }
        catch (Throwable ex) {}
        return false;
    }

    @Override
    protected final ActionTaker getActionTaker () {
        return ACTION_TAKER;
    }
}