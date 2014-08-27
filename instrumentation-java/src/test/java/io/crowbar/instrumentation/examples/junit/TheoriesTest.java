package io.crowbar.instrumentation.examples.junit;

import static org.junit.Assert.assertTrue;

import org.junit.Assume;
import org.junit.experimental.theories.DataPoints;
import org.junit.experimental.theories.Theories;
import org.junit.experimental.theories.Theory;
import org.junit.runner.RunWith;

@RunWith(Theories.class)
public class TheoriesTest {
    @DataPoints
    public static int[] input () {
        return new int[] {
                   -1, -10, -1234567, 1, 10, 1234567
        };
    }

    @Theory
    public void test (Integer a,
                      Integer b) {
        Assume.assumeTrue(a > 0 && b > 0);
        assertTrue(a + b > a);
        assertTrue(a + b > b);
    }
}