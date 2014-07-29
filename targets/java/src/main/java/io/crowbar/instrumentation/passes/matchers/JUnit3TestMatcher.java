package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;


public class JUnit3TestMatcher implements Matcher {
    private static final Matcher MATCHER = new AndMatcher( 
    		new SuperclassMatcher("junit.framework.TestCase"),
    		new PrefixMatcher("test"));

    @Override
    public final boolean matches (CtClass c) {
        return false;
    }

    @Override
    public final boolean matches (CtClass c, CtMethod m) {
        return MATCHER.matches(c,m);
    }
}