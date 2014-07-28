package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;


public class ModifierMatcher implements Matcher {
    private final int modifier_mask;

    public ModifierMatcher (int modifier_mask) {
        this.modifier_mask = modifier_mask;
    }

    @Override
    public boolean matches (CtClass c) {
        return matches(c.getModifiers());
    }

    @Override
    public boolean matches (CtMethod m) {
        return matches(m.getModifiers());
    }

    private boolean matches (int modifier) {
        return (modifier & modifier_mask) != 0;
    }
}