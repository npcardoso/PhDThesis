package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AndMatcher;
import io.crowbar.instrumentation.passes.matchers.ArgumentTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.ClassAnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.ModifierMatcher;
import io.crowbar.instrumentation.passes.matchers.MethodAnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.NotMatcher;
import io.crowbar.instrumentation.passes.matchers.OrMatcher;
import io.crowbar.instrumentation.passes.matchers.ReturnTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;
import io.crowbar.instrumentation.runtime.Collector;

import java.lang.annotation.Annotation;
import java.lang.reflect.Method;
import java.util.regex.Pattern;

import javassist.CtClass;
import javassist.CtMethod;
import javassist.Modifier;

import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


public final class TestNGTestWrapper extends AbstractTestWrapper {
    private static final Logger logger = LogManager.getLogger(TestNGTestWrapper.class);

    private static final String ANNOTATION_CLASS = "org.testng.annotations.Test";
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
            new AndMatcher(
                new NotMatcher( // A test method must not have any of the following annotations
                    new OrMatcher(
                        new MethodAnnotationMatcher("org.testng.annotations.BeforeSuite"),
                        new MethodAnnotationMatcher("org.testng.annotations.AfterSuite"),
                        new MethodAnnotationMatcher("org.testng.annotations.BeforeTest"),
                        new MethodAnnotationMatcher("org.testng.annotations.AfterTest"),
                        new MethodAnnotationMatcher("org.testng.annotations.BeforeGroups"),
                        new MethodAnnotationMatcher("org.testng.annotations.AfterGroups"),
                        new MethodAnnotationMatcher("org.testng.annotations.BeforeClass"),
                        new MethodAnnotationMatcher("org.testng.annotations.AfterClass"),
                        new MethodAnnotationMatcher("org.testng.annotations.BeforeMethod"),
                        new MethodAnnotationMatcher("org.testng.annotations.AfterMethod"))),
                new ArgumentTypeMatcher(), // A test method must have no parameters
                new OrMatcher( // A test method must be annotated or be in annotated class
                    new ClassAnnotationMatcher(ANNOTATION_CLASS),
                    new MethodAnnotationMatcher(ANNOTATION_CLASS)),
                new ReturnTypeMatcher("void"), // A test method must return void
                new ModifierMatcher(Modifier.PUBLIC))); // A test method must be public


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
        boolean ret = true;


        try {
            Method method = cls.getMethod(methodName);
            Annotation an = getAnnotation(method);

            if (an == null) {
                ret = true;
            } else {
                Class< ? >[] expected = getExpectedEx(an);
                String expectedMsgRegex = getExpectedMsgRegex(an);

                ret = (expected.length == 0 && Pattern.matches(expectedMsgRegex, ""));
            }
        }
        catch (Throwable ex) {
            logger.warn(ex, ex);
        }

        logger.debug("isPass({},{},{}) = {}",
                     cls.getName(),
                     methodName,
                     ret);

        return ret;
    }

    public static final boolean isPass (Class cls,
                                        String methodName,
                                        Throwable e) {
        boolean ret = false;


        try {
            Method method = cls.getMethod(methodName);
            Annotation an = getAnnotation(method);

            if (an == null)
                return false;

            Class< ? >[] expected = getExpectedEx(an);
            String expectedMsgRegex = getExpectedMsgRegex(an);
            String msg = e.getMessage() == null ? "" : e.getMessage();

            for (Class c : expected) {
                if (isSameType(e, c) &&
                    Pattern.matches(expectedMsgRegex, msg)) {
                    ret = true;
                    break;
                }
            }
        }
        catch (Throwable ex) {
            logger.warn(ex, ex);
        }

        logger.debug("isPass({},{},{}) = {}",
                     cls.getName(),
                     methodName,
                     e.getClass().getName(),
                     ret);

        return ret;
    }

    @Override
    protected final ActionTaker getActionTaker () {
        return ACTION_TAKER;
    }
}