package io.crowbar.instrumentation;

import io.crowbar.diagnostic.spectrum.Node;

import static org.junit.Assert.assertEquals;

import org.junit.Assert;
import org.junit.Test;

public class RegisterNodeMessageTest {
    @Test
    public void testEmptyConstructor () {
        Messages.RegisterNodeMessage mr = new Messages.RegisterNodeMessage();
        Assert.assertNull(mr.getName());
        assertEquals(-1, mr.getNodeId());
        assertEquals(-1, mr.getParentId());
    }

    @Test
    public void testNonEmptyConstructor () {
        Messages.RegisterNodeMessage mr = new Messages.RegisterNodeMessage(0, 1, "node message", Node.Type.CLASS);
        assertEquals("node message", mr.getName());
        assertEquals(0, mr.getNodeId());
        assertEquals(1, mr.getParentId());
        assertEquals(Node.Type.CLASS, mr.getType());
    }
}