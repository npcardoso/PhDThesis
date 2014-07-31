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
    private static final ActionTaker actionTaker =
        new WhiteList(
            new AndMatcher(
                new SuperclassMatcher("junit.framework.TestCase"),
                new PrefixMatcher("test")));

    @Override
    public Action getAction (CtClass c) {
        return actionTaker.getAction(c);
    }

    @Override
    public Action getAction (CtClass c, CtMethod m) {
        return actionTaker.getAction(c, m);
    }

    @Override
    public Set<String> validExceptions (CtClass c,
                                        CtMethod m) {
        return null;
    }
}