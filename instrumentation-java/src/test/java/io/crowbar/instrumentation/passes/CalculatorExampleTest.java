package io.crowbar.instrumentation.passes;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class CalculatorExampleTest {
    @Test
    public void testGetName () {
        CalculatorExample c = new CalculatorExample("calc");


        assertEquals("calc", c.getName());
    }

    public void testAdd () {
        CalculatorExample c = new CalculatorExample("calc");


        assertEquals(5, c.add(3, 2));
    }
}