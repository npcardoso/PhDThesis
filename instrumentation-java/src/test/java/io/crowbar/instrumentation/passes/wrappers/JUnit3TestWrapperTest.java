package io.crowbar.instrumentation.passes.wrappers;

import org.junit.Test;

public class JUnit3TestWrapperTest {
    @Test(expected = NullPointerException.class)
    public void testNullActionTaker () {
        JUnit3TestWrapper jW = new JUnit3TestWrapper();


        jW.getAction(null);
    }
}