package io.crowbar.instrumentation.passes.wrappers;

import static org.junit.Assert.assertEquals;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.NotFoundException;

import org.junit.Before;
import org.junit.Test;

public class JUnit4TestWrapperTest {
    private static String testClass = "io.crowbar.instrumentation.passes.CalculatorExampleTest";

    private ClassPool cp;
    private CtClass cc;

    @Before
    public void setUp () throws NotFoundException {
        this.cp = ClassPool.getDefault();
        this.cc = this.cp.get(testClass);
    }

    @Test
    public void testGetAction () {
        JUnit4TestWrapper jW = new JUnit4TestWrapper();


        assertEquals(ActionTaker.Action.NEXT, jW.getAction(this.cc, null));
        assertEquals(ActionTaker.Action.NEXT, jW.getAction(this.cc));
    }

    @Test
    public void testIsDefaultPass () {
        JUnit4TestWrapper jW = new JUnit4TestWrapper();


        assertEquals(true, jW.isDefaultPass(this.cc, null));
    }

    @Test
    public void testIsPass () {
        assertEquals(false, JUnit4TestWrapper.isPass(null, 0, null, ""));
    }
}