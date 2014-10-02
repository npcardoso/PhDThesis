package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AndMatcher;
import io.crowbar.instrumentation.passes.matchers.OrMatcher;
import io.crowbar.instrumentation.passes.matchers.MethodAnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.ReturnTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;
import io.crowbar.instrumentation.runtime.Collector;


import javassist.CtClass;
import javassist.CtMethod;

import java.lang.annotation.Annotation;
import java.lang.reflect.Method;

import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


public final class JUnit4TestWrapper
extends AbstractTestWrapper {
    private static final Logger logger = LogManager.getLogger(JUnit4TestWrapper.class);
    private static final String ANNOTATION_CLASS = "org.junit.Test";
    private static final String EXPECTED_NONE_CLASS = ANNOTATION_CLASS + "$None";
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
            new AndMatcher(
                new OrMatcher(
                    new MethodAnnotationMatcher(ANNOTATION_CLASS),
                    new MethodAnnotationMatcher("org.junit.experimental.theories.Theory")),
                new ReturnTypeMatcher("void")));


    private static Class getExpected (Method m) {
        try {
            Annotation an = m.getAnnotation((Class< ? extends Annotation> )Class.forName(ANNOTATION_CLASS));
            Method method = an.getClass().getMethod("expected");
            return (Class) method.invoke(an);
        }
        catch (Throwable e) {
            logger.warn(e, e);
        }

        return null;
    }

    public static boolean isPass (Class cls,
                                  String methodName) {
        boolean ret = true;


        try {
            Method method = cls.getMethod(methodName);
            Class c = getExpected(method);
            ret = (c == null || c.getName() == EXPECTED_NONE_CLASS);
        }
        catch (NoSuchMethodException e) {
            logger.warn(e, e);
        }
        logger.debug("isPass({},{}) = {}",
                     cls.getName(),
                     methodName,
                     ret);

        return ret;
    }

    public static boolean isPass (Class cls,
                                  String methodName,
                                  Throwable e) {
        boolean ret = false;


        if (e == null)
            ret = true;
        else if (isSameType(e, "org.junit.Assume$AssumptionViolatedException"))
            ret = true;
        else if (isSameType(e, "org.junit.internal.AssumptionViolatedException"))
            ret = true;
        else
            try {
                Method method = cls.getMethod(methodName);
                Class expected = getExpected(method);
                ret = isSameType(e, expected);
            }
            catch (NoSuchMethodException ex) {
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
    protected ActionTaker getActionTaker () {
        return ACTION_TAKER;
    }
}