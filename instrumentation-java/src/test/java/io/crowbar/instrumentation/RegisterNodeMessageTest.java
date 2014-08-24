package io.crowbar.instrumentation;

import static org.junit.Assert.assertEquals;
import junit.framework.Assert;

import org.junit.Test;

public class RegisterNodeMessageTest {
    @Test
    public void testEmptyConstructor () {
        Messages.RegisterNodeMessage mr = new Messages.RegisterNodeMessage();
        Assert.assertNull(mr.getName());
        assertEquals(-1, mr.getId());
        assertEquals(-1, mr.getParentId());
    }

    @Test
    public void testNonEmptyConstructor () {
        Messages.RegisterNodeMessage mr = new Messages.RegisterNodeMessage("node message", 0, 1);
        assertEquals("node message", mr.getName());
        assertEquals(0, mr.getId());
        assertEquals(1, mr.getParentId());

        assertEquals("[[RegisterNodeMessage]: name: node message] id: 0] parentId: 1]", mr.toString());
    }
}