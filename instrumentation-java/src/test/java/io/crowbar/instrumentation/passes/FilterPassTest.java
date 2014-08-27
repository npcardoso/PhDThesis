package io.crowbar.instrumentation.passes;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertSame;
import io.crowbar.instrumentation.passes.Pass.Outcome;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.BlackList;
import io.crowbar.instrumentation.passes.matchers.NotMatcher;
import io.crowbar.instrumentation.passes.matchers.ReturnTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.SuffixMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;

import java.util.List;

import javassist.CtClass;

import org.junit.Test;

public class FilterPassTest {
    @Test(expected = NullPointerException.class)
    public void testNewFilterWithNullArray ()  throws Throwable  {
        new FilterPass((List<ActionTaker> ) null);
    }

    @Test
    public void testNullClass () throws Exception {
        FilterPass fp = new FilterPass();


        assertEquals(Outcome.CONTINUE, fp.transform(null));
    }

    @Test
    public void testAbort () throws Exception {
        FilterPass fp = new FilterPass(new ActionTaker[] {
                                           new BlackList(
                                               new SuffixMatcher(""))
                                       });


        assertEquals(Pass.Outcome.ABORT, fp.transform(CtClass.longType));
    }

    @Test
    public void testContinue () throws Exception {
        FilterPass fp = new FilterPass(new ActionTaker[] {
                                           new WhiteList(
                                               new NotMatcher(
                                                   new ReturnTypeMatcher("dummy")))
                                       });


        assertEquals(Pass.Outcome.CONTINUE, fp.transform(CtClass.intType));
    }

    @Test
    public void testSetFallbackOutcome () {
        FilterPass fp1 = new FilterPass();
        FilterPass fp2 = fp1.setFallbackOutcome(Pass.Outcome.RETURN);


        assertSame(fp2, fp1);
    }

    @Test
    public void testSetAcceptOutcome () {
        FilterPass fp1 = new FilterPass();
        FilterPass fp2 = fp1.setAcceptOutcome(Pass.Outcome.RETURN);


        assertSame(fp2, fp1);
    }

    @Test
    public void testSetReject () {
        FilterPass fp1 = new FilterPass();
        FilterPass fp2 = fp1.setReject(Pass.Outcome.ABORT);


        assertSame(fp2, fp1);
    }
}