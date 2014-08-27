package io.crowbar.instrumentation.passes;

import static org.junit.Assert.assertEquals;
import io.crowbar.instrumentation.examples.Dummy;
import io.crowbar.instrumentation.passes.InjectPass.Granularity;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.NotFoundException;

import org.junit.Before;
import org.junit.Test;

public class InjectPassTest {
    private static String testClass = Dummy.class.getCanonicalName();

    private ClassPool cp;
    private CtClass cc;

    @Before
    public void setUp () throws NotFoundException {
        this.cp = ClassPool.getDefault();
        this.cc = this.cp.get(testClass);
    }

    @Test
    public void testFunctionGranularity () throws Exception {
        InjectPass ip = new InjectPass(Granularity.FUNCTION);


        ip.transform(this.cc);

        assertEquals(1, cc.getFields().length);
        assertEquals("__CROWBAR_HIT_VECTOR__", this.cc.getFields()[0].getName());
    }
}