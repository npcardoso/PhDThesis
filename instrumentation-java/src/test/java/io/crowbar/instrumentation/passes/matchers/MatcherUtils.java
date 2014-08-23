package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;

public final class MatcherUtils {
    public static class TrueMatcher implements Matcher {
        @Override
        public boolean matches (CtClass c) {
            return true;
        }

        @Override
        public boolean matches (CtClass c, CtMethod m) {
            return true;
        }
    }

    public static class FalseMatcher implements Matcher {
        @Override
        public boolean matches (CtClass c) {
            return false;
        }

        @Override
        public boolean matches (CtClass c, CtMethod m) {
            return false;
        }
    }
}