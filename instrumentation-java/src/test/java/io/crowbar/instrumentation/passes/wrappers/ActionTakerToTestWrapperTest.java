package io.crowbar.instrumentation.passes.wrappers;

import javassist.ClassPool;
import javassist.CtClass;
import javassist.NotFoundException;

import org.junit.Before;
import org.junit.Test;

public class ActionTakerToTestWrapperTest {
    private static String testClass = "io.crowbar.instrumentation.passes.CalculatorExampleTest";

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
}