package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;
import javassist.NotFoundException;

public class OverrideMatcher implements Matcher {
    private final Matcher matcher;
    private final Boolean overrideClass;
    private final Boolean overrideMethod;

    public OverrideMatcher (Matcher matcher,
                            Boolean overrideClass,
                            Boolean overrideMethod) {
        this.matcher = matcher;
        this.overrideClass = overrideClass;
        this.overrideMethod = overrideMethod;
    }

    @Override
    public final boolean matches (CtClass c) {
        if (overrideClass != null)
            return overrideClass;

        return matcher.matches(c);
    }

    @Override
    public final boolean matches (CtClass c,
                                  CtMethod m) {
        if (overrideMethod != null)
            return overrideMethod;

        return matcher.matches(c, m);
    }
}