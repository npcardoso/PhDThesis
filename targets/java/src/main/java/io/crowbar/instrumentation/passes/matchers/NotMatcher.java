package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;

public class NotMatcher implements Matcher {
    private final Matcher matcher;
    public NotMatcher (Matcher matcher) {
        this.matcher = matcher;
    }

    @Override
    public boolean matches (CtClass c) {
        return !matcher.matches(c);
    }

    @Override
    public boolean matches (CtMethod m) {
        return !matcher.matches(m);
    }
}