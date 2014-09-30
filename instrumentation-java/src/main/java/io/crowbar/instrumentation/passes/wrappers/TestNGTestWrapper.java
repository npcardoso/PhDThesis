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


public final class TestNGTestWrapper extends AbstractTestWrapper {
    private static final String ANNOTATION_CLASS = "org.testng.annotations.Test";
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
            new AndMatcher(
                new AnnotationMatcher(ANNOTATION_CLASS),
                new ReturnTypeMatcher("void")));


    private static Annotation getAnnotation (Method method) throws Exception {
        return method.getAnnotation((Class< ? extends Annotation> )Class.forName(ANNOTATION_CLASS));
    }

    private static Class< ? >[] getExpectedEx (Annotation annotation) throws Exception {
        Method method = annotation.getClass().getMethod("expectedExceptions");


        Class< ? >[] expected = (Class[])method.invoke(annotation);

        expected = expected == null ? new Class[] {} : expected;


        return expected;
    }

    private static String getExpectedMsgRegex (Annotation annotation) throws Exception {
        Method method = annotation.getClass().getMethod("expectedExceptionsMessageRegExp");

        String expectedMsgRegex = (String) method.invoke(annotation);


        expectedMsgRegex = expectedMsgRegex == null ? ".*" : expectedMsgRegex;

        return expectedMsgRegex;
    }

    public static final boolean isPass (Class cls,
                                        String methodName) {
        try {
            Method method = cls.getMethod(methodName);
            Annotation an = getAnnotation(method);
            Class< ? >[] expected = getExpectedEx(an);
            String expectedMsgRegex = getExpectedMsgRegex(an);

            return expected.length == 0 && Pattern.matches(expectedMsgRegex, "");
        }
        catch (Throwable ex) {
            ex.printStackTrace();
        }
        return true;
    }

    public static final boolean isPass (Class cls,
                                        String methodName,
                                        Throwable e) {
        try {
            Method method = cls.getMethod(methodName);
            Annotation an = getAnnotation(method);
            Class< ? >[] expected = getExpectedEx(an);
            String expectedMsgRegex = getExpectedMsgRegex(an);
            String msg = e.getMessage() == null ? "" : e.getMessage();

            for (Class c : expected) {
                if (isSameType(e, c) &&
                    Pattern.matches(expectedMsgRegex, msg))
                    return true;
            }
        }
        catch (Throwable ex) {
            ex.printStackTrace();
        }
        return false;
    }

    @Override
    protected final ActionTaker getActionTaker () {
        return ACTION_TAKER;
    }
}