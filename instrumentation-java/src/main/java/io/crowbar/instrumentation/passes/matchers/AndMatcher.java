package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;

public class AndMatcher implements Matcher {
    private final Matcher matcherA;
    private final Matcher matcherB;

    public AndMatcher (Matcher matcherA,
                       Matcher matcherB) {
        this.matcherA = matcherA;
        this.matcherB = matcherB;
    }

    @Override
    public final boolean matches (CtClass c) {
        return matcherA.matches(c) && matcherB.matches(c);
    }

    @Override
    public final boolean matches (CtClass c, CtMethod m) {
        return matcherA.matches(c,m) && matcherB.matches(c,m);
    }
}