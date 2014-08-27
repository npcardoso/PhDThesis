package io.crowbar.instrumentation.passes.wrappers;

import static org.junit.Assert.assertEquals;
import io.crowbar.instrumentation.examples.junit.TimeoutTest;
import io.crowbar.instrumentation.passes.matchers.ActionTaker.Action;
import io.crowbar.instrumentation.passes.matchers.ReturnTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;
import javassist.NotFoundException;
import javassist.bytecode.Descriptor;

import org.junit.Before;
import org.junit.Test;

public class ActionTakerToTestWrapperTest {
    private static String testClass = TimeoutTest.class.getCanonicalName();

    private ClassPool cp;
    private CtClass cc;

    @Before
    public void setUp () throws NotFoundException {
        this.cp = ClassPool.getDefault();
        this.cc = this.cp.get(testClass);
    }

    @Test(expected = NullPointerException.class)
    public void testNullActionTaker () {
        ActionTakerToTestWrapper atw = new ActionTakerToTestWrapper(null);


        atw.getAction(this.cc);
    }

    @Test
    public void testGetAction () throws NotFoundException {
        ActionTakerToTestWrapper atw = new ActionTakerToTestWrapper(
            new WhiteList(
                new ReturnTypeMatcher("void")));


        assertEquals(Action.NEXT, atw.getAction(this.cc));

        CtMethod cm = cc.getMethod("testWithLocalTimeout", Descriptor.ofMethod(CtClass.voidType, null));
        assertEquals(Action.ACCEPT, atw.getAction(this.cc, cm));
    }
}