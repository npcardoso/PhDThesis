package io.crowbar.instrumentation.passes;

import static org.junit.Assert.assertEquals;
import io.crowbar.instrumentation.events.AbstractEventListener;
import io.crowbar.instrumentation.examples.junit.AssumeTest;
import io.crowbar.instrumentation.passes.wrappers.JUnit4TestWrapper;
import io.crowbar.instrumentation.passes.wrappers.TestWrapper;
import io.crowbar.instrumentation.runtime.Collector;

import java.util.LinkedList;
import java.util.List;

import javassist.ClassPool;
import javassist.CtClass;
import javassist.NotFoundException;

import org.junit.Before;
import org.junit.Test;

public class TestWrapperPassTest {
    private static String testClass = AssumeTest.class.getCanonicalName();

    private ClassPool cp;
    private CtClass cc;

    @Before
    public void setUp () throws NotFoundException {
        Collector.start("", new AbstractEventListener() {});
        this.cp = ClassPool.getDefault();
        this.cc = this.cp.get(testClass);
    }

    @Test
    public void testTransform () throws Exception {
        List<TestWrapper> ltw = new LinkedList<TestWrapper> ();
        ltw.add(new JUnit4TestWrapper());

        TestWrapperPass twp = new TestWrapperPass(ltw);
        assertEquals(Pass.Outcome.CONTINUE, twp.transform(this.cc));
    }
}