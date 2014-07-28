package io.crowbar.instrumentation.passes.matchers;

import java.util.Arrays;
import java.util.List;
import java.util.LinkedList;

import javassist.CtClass;
import javassist.CtMethod;


public class SuffixMatcher implements Matcher {
    private List<String> suffix = new LinkedList<String> ();

    public SuffixMatcher (String... strings) {
        suffix.addAll(Arrays.asList(strings));
    }

    public SuffixMatcher (List<String> strings) {
        suffix.addAll(strings);
    }

    @Override
    public final boolean matches (CtClass c) {
        return matches(c.getName());
    }

    @Override
    public final boolean matches (CtMethod m) {
        return matches(m.getName());
    }

    private boolean matches (String name) {
        for (String s : suffix) {
            if (name.endsWith(s))
                return true;
        }

        return false;
    }
}