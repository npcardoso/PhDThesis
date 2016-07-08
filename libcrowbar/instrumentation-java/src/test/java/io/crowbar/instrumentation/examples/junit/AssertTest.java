package io.crowbar.instrumentation.examples.junit;

import static org.hamcrest.CoreMatchers.not;
import static org.hamcrest.CoreMatchers.sameInstance;
import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNotSame;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertThat;
import static org.junit.Assert.assertTrue;

import org.hamcrest.core.IsEqual;
import org.hamcrest.core.IsNull;
import org.junit.FixMethodOrder;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runners.MethodSorters;

@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class AssertTest {
    @Test
    public void testAssertArrayEquals () {
        int[] expected = {
            0, 1, 2
        };
        int[] actual = {
            0, 1, 2
        };
        assertArrayEquals(expected, actual);
    }

    @Test
    public void testAssertEquals () {
        assertEquals(0, 0);
    }

    @Test
    public void testAssertFalse () {
        assertFalse(false);
    }

    @Test
    public void testAssertTrue () {
        assertTrue(true);
    }

    @Test
    public void testAssertNotNull () {
        assertNotNull(new String());
    }

    @Test
    public void testAssertNotSame () {
        assertNotSame(new String(), new String());
    }

    @Test
    public void testAssertNull () {
        assertNull(null);
    }

    @Test
    public void testAssertSame () {
        String s = new String("asd");


        assertSame(s, s);
    }

    @Test
    public void testAssertMatchers () {
        assertThat(new Object(), not(sameInstance(new Object())));
        assertThat(null, new IsNull<Object> ());
        assertThat(0, new IsEqual<Integer> (0));
    }

    @Ignore
    @Test
    public void testSane () {
        assertTrue(true);
    }
}