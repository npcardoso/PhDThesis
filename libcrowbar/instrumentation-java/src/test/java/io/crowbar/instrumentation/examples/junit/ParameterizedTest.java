package io.crowbar.instrumentation.examples.junit;

import static org.junit.Assert.assertEquals;
import io.crowbar.instrumentation.examples.Dummy;

import java.util.Arrays;
import java.util.Collection;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameter;

@RunWith(Parameterized.class)
public class ParameterizedTest {
    @Parameter
    public int a;

    @Parameter(value = 1)
    public int b;

    @Parameter(value = 2)
    public int expected;

    public ParameterizedTest () {
        // empty
    }

    @Parameterized.Parameters
    public static Collection< ? > primeNumbers () {
        return Arrays.asList(new Object[][] {
                                 {0, 1, 0},
                                 {2, 1, 2},
                                 {2, 2, 4},
                                 {3, 2, 6}
                             });
    }

    @Test
    public void test () {
        assertEquals(expected, Dummy.dummy(a, b));
    }
}