package io.crowbar.sandbox;

import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.WritableTree;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import static org.junit.Assert.*;
import org.junit.Test;


public class TreeTest {
    @Test
    public void TestAddChild () throws RegistrationException {
        WritableTree t = new WritableTree("");

        Node n = t.getRoot();


        assertEquals(n.isRoot(), true);

        assertNotNull(n);

        Node c1 = t.addNode("foo1", n);
        Node c2 = t.addNode("foo2", n);

        assertEquals(n, c1.getParent());
        assertEquals(n, c2.getParent());

        assertEquals(c1, n.getChild("foo1"));
        assertEquals(c2, n.getChild("foo2"));


        Node c11 = t.addNode("foo1", c1);
        Node c12 = t.addNode("foo2", c1);
        Node c21 = t.addNode("foo1", c2);
        Node c22 = t.addNode("foo2", c2);

        assertEquals(c1, c11.getParent());
        assertEquals(c1, c12.getParent());
        assertEquals(c2, c21.getParent());
        assertEquals(c2, c22.getParent());

        assertEquals(c11, c1.getChild("foo1"));
        assertEquals(c12, c1.getChild("foo2"));
        assertEquals(c21, c2.getChild("foo1"));
        assertEquals(c22, c2.getChild("foo2"));
    }
}