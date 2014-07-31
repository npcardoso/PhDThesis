package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AndMatcher;
import io.crowbar.instrumentation.passes.matchers.PrefixMatcher;
import io.crowbar.instrumentation.passes.matchers.SuperclassMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;

import java.util.Set;

import javassist.CtClass;
import javassist.CtMethod;


public class JUnit3TestWrapper implements TestWrapper {
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
            new AndMatcher(
                new SuperclassMatcher("junit.framework.TestCase"),
                new PrefixMatcher("test")));

    @Override
    public final Action getAction (CtClass c) {
        return ACTION_TAKER.getAction(c);
    }

    @Override
    public final Action getAction (CtClass c,
                                   CtMethod m) {
        return ACTION_TAKER.getAction(c, m);
    }

    @Override
    public final Set<String> validExceptions (CtClass c,
                                              CtMethod m) {
        return null;
    }
}