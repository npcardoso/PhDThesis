package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;
import javassist.NotFoundException;


public class SuperclassMatcher implements Matcher {
    private final String superclass;

    public SuperclassMatcher (String superclass) {
        this.superclass = superclass;
    }

    @Override
    public final boolean matches (CtClass c) {
        return matchesSuperclass (c);
    }

    @Override
    public final boolean matches (CtClass c, CtMethod m) {
        return matchesSuperclass (c);
    }

    private boolean matchesSuperclass (CtClass c) {
    	try {
    		String sc = c.getSuperclass().getName();
    		return superclass.equals(sc);
    	} catch (NotFoundException e) {
    		return false;
    	}
    }
}