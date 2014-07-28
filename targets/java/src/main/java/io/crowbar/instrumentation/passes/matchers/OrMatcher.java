package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;

public class OrMatcher implements Matcher {
    private final Matcher matcherA;
    private final Matcher matcherB;

    public OrMatcher (Matcher matcherA,
                      Matcher matcherB) {
        this.matcherA = matcherA;
        this.matcherB = matcherB;
    }

    @Override
    public boolean matches (CtClass c) {
        return matcherA.matches(c) || matcherB.matches(c);
    }

    @Override
    public boolean matches (CtMethod m) {
        return matcherA.matches(m) || matcherB.matches(m);
    }
}