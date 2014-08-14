package io.crowbar.instrumentation.runtime;

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

    @Test
    public void TestFullName () throws RegistrationException {
        WritableTree t = new WritableTree("foo0");
        int count = 4;
        Node n = t.getRoot();
        String ret = "foo0";


        for (int i = 1; i < count; i++) {
            n = t.addNode("foo" + i, n);
            ret += ":foo" + i;
        }

        assertEquals(ret, n.getFullName(":"));

        for (int i = 0; i < count; i++) {
            ret = "";
            Node tmp = n;

            for (int j = i; j < count; j++) {
                ret = tmp.getName() + ":" + ret;
                tmp = tmp.getParent();
            }

            assertEquals(ret, n.getFullName(":", i) + ":");
        }
    }
}