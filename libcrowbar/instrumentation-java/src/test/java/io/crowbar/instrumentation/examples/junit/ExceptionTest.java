package io.crowbar.instrumentation.examples.junit;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import java.util.ArrayList;
import java.util.List;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class ExceptionTest {
    @Rule
    public ExpectedException thrown = ExpectedException.none();

    @Test
    public void testTryCatch () {
        try {
            new ArrayList<Object> ().get(0);
            fail("Expected an IndexOutOfBoundsException to be thrown");
        }
        catch (IndexOutOfBoundsException anIndexOutOfBoundsException) {
            assertEquals("Index: 0, Size: 0", anIndexOutOfBoundsException.getMessage());
        }
    }

    @Test
    public void testRuleException () throws IndexOutOfBoundsException {
        List<Object> list = new ArrayList<Object> ();

        thrown.expect(IndexOutOfBoundsException.class);
        thrown.expectMessage("Index: 0, Size: 0");
        list.get(0);
    }

    @Test(expected = IndexOutOfBoundsException.class)
    public void testExpected () {
        List<Object> list = new ArrayList<Object> ();
        list.get(0);
    }
}