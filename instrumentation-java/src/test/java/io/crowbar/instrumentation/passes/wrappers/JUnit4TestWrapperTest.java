package io.crowbar.instrumentation.passes.wrappers;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import io.crowbar.instrumentation.examples.junit.ExceptionTest;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;
import javassist.NotFoundException;
import javassist.bytecode.Descriptor;

import org.junit.Before;
import org.junit.Test;
import org.junit.internal.AssumptionViolatedException;

public class JUnit4TestWrapperTest {
    private static String testClass = ExceptionTest.class.getCanonicalName();

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
    public void testIsDefaultPassWithoutExpected () {
        JUnit4TestWrapper jW = new JUnit4TestWrapper();


        assertEquals(true, jW.isDefaultPass(this.cc, null));
    }

    @Test
    public void testIsDefaultPassWithExpected () throws NotFoundException {
        JUnit4TestWrapper jW = new JUnit4TestWrapper();
        CtMethod cm = cc.getMethod("testExpected", Descriptor.ofMethod(CtClass.voidType, null));


        assertEquals(false, jW.isDefaultPass(this.cc, cm));
    }

    @Test
    public void testIsPassAssumptionViolatedException () {
        assertEquals(true, JUnit4TestWrapper.isPass(new AssumptionViolatedException(""), ""));
    }

    @Test
    public void testIsPassNullPointerException () {
        assertEquals(true, JUnit4TestWrapper.isPass(new NullPointerException(""), java.lang.NullPointerException.class.getCanonicalName()));
    }

    @Test
    public void testOracleCodeWithoutExpected () throws NotFoundException {
        JUnit4TestWrapper jW = new JUnit4TestWrapper();
        CtMethod cm = cc.getMethod("testTryCatch", Descriptor.ofMethod(CtClass.voidType, null));


        assertTrue(jW.getOracleCode(this.cc, cm, null, 0, "", "").contains(", 0, , \"\""));
    }

    @Test
    public void testOracleCodeWithExpected () throws NotFoundException {
        JUnit4TestWrapper jW = new JUnit4TestWrapper();
        CtMethod cm = cc.getMethod("testExpected", Descriptor.ofMethod(CtClass.voidType, null));


        assertTrue(jW.getOracleCode(this.cc, cm, null, 0, "", "").contains("\"java.lang.IndexOutOfBoundsException\""));
    }
}