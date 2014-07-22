package io.crowbar.sandbox;

import io.crowbar.sandbox.Tree.Node;

import static org.junit.Assert.*;
import org.junit.Test;


public class TreeTest {
    @Test
    public void TestAddChild () {
        Tree t = new Tree();

        Node n = t.getRoot();


        assertNotNull(n);

        Node c1 = n.addChild("foo1");
        Node c2 = n.addChild("foo2");

        assertEquals(n, c1.getParent());
        assertEquals(n, c2.getParent());

        assertEquals(c1, n.getChild("foo1"));
        assertEquals(c2, n.getChild("foo2"));


        Node c11 = c1.addChild("foo1");
        Node c12 = c1.addChild("foo2");
        Node c21 = c2.addChild("foo1");
        Node c22 = c2.addChild("foo2");

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