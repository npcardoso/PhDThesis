package io.crowbar.instrumentation.examples.junit;

import static org.junit.Assert.assertFalse;

import org.junit.Assume;
import org.junit.Test;

public class AssumeTest {
    @SuppressWarnings("null")
    @Test
    public void testAssumeTrue () {
        Class< ? > cls = null;
        try {
            Class.forName("random.class.which.does.not.exist.JavaClass");
        }
        catch (ClassNotFoundException e) {
            // empty
        }
        Assume.assumeNotNull(cls);
        assertFalse(cls.isEnum());
    }
}