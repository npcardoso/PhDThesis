package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.*;

import java.util.*;
import javassist.*;

public class IgnorePass extends Pass {
    public IgnorePass (boolean blacklist) {
        this.blacklist = blacklist;
    }

    @Override
    public void transform (CtClass c,
                           ProbeSet ps) throws Exception {
        boolean matches =
            checkPrefixes(c) ||
            checkSuffixes(c) ||
            checkModifiers(c);


        if (matches && blacklist)
            throw new IgnoreClassException();

        if (!matches && !blacklist)
            throw new IgnoreClassException();
    }

    private boolean checkSuffixes (CtClass c) {
        for (String s : suffix)
            if (c.getName().endsWith(s))
                return true;

        return false;
    }

    private boolean checkPrefixes (CtClass c) {
        for (String s : prefix)
            if (c.getName().startsWith(s))
                return true;

        return false;
    }

    private boolean checkModifiers (CtClass c) {
        return (c.getModifiers() & modifier_mask) != 0;
    }

    private boolean blacklist;
    public List<String> prefix = new LinkedList<String> ();
    public List<String> suffix = new LinkedList<String> ();
    public int modifier_mask = 0;
}