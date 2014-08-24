package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.assertEquals;
import io.crowbar.instrumentation.passes.matchers.ActionTaker.Action;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.FalseMatcher;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.TrueMatcher;
import javassist.CtClass;
import javassist.CtMethod;

import org.junit.Test;

public class BlackListTest {
    final private Matcher trueMatcher = new TrueMatcher();
    final private Matcher falseMatcher = new FalseMatcher();

    @Test
    public void testGetAction () {
        BlackList bl = new BlackList(null);


        assertEquals(bl.getAction(false), Action.NEXT);
        assertEquals(bl.getAction(true), Action.REJECT);
    }

    @Test
    public void testGetActionClass () {
        BlackList bl = new BlackList(trueMatcher);


        assertEquals(bl.getAction((CtClass) null), Action.REJECT);
    }

    @Test
    public void testGetActionClass2 () {
        BlackList bl = new BlackList(falseMatcher);


        assertEquals(bl.getAction((CtClass) null), Action.NEXT);
    }

    @Test
    public void testGetActionMethod () {
        BlackList bl = new BlackList(trueMatcher);


        assertEquals(bl.getAction((CtClass) null, (CtMethod) null), Action.REJECT);
    }

    @Test
    public void testGetActionMethod2 () {
        BlackList bl = new BlackList(falseMatcher);


        assertEquals(bl.getAction((CtClass) null, (CtMethod) null), Action.NEXT);
    }
}