package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import io.crowbar.instrumentation.passes.matchers.ActionTaker.Action;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.FalseMatcher;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.TrueMatcher;
import javassist.CtClass;
import javassist.CtMethod;

import org.junit.Test;

public class WhiteListTest {
    final private Matcher trueMatcher = new TrueMatcher();
    final private Matcher falseMatcher = new FalseMatcher();

    @Test
    public void testGetAction () {
        WhiteList wl = new WhiteList(null);


        assertEquals(wl.getAction(false), Action.NEXT);
        assertEquals(wl.getAction(true), Action.ACCEPT);
    }

    @Test
    public void testGetActionClass () {
        WhiteList wl = new WhiteList(trueMatcher);


        assertEquals(wl.getAction((CtClass) null), Action.ACCEPT);
    }

    @Test
    public void testGetActionClass2 () {
        WhiteList wl = new WhiteList(falseMatcher);


        assertEquals(wl.getAction((CtClass) null), Action.NEXT);
    }

    @Test
    public void testGetActionMethod () {
        WhiteList wl = new WhiteList(trueMatcher);


        assertEquals(wl.getAction((CtClass) null, (CtMethod) null), Action.ACCEPT);
    }

    @Test
    public void testGetActionMethod2 () {
        WhiteList wl = new WhiteList(falseMatcher);


        assertEquals(wl.getAction((CtClass) null, (CtMethod) null), Action.NEXT);
    }
}