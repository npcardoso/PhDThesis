package io.crowbar.diagnostic.spectrum;

import static org.junit.Assert.assertEquals;

import org.junit.BeforeClass;
import org.junit.Test;

public class NodeUtilsTest {
    static Node class1, class2, class3, innerClass1;
    static Node method1, method2, method3;
    static Node line1, line2, line3;

    @BeforeClass
    public static void setup () {
        EditableTree et = new EditableTree("root");

        Node package1 = et.addNode("package1", Node.Type.PACKAGE, et.getRoot().getId());
        Node package2 = et.addNode("package2", Node.Type.PACKAGE, et.getRoot().getId());
        Node package3 = et.addNode("package3", Node.Type.PACKAGE, package2.getId());


        class1 = et.addNode("Class1", Node.Type.CLASS, package1.getId());
        class2 = et.addNode("Class2", Node.Type.CLASS, package3.getId());
        class3 = et.addNode("Class3", Node.Type.CLASS, et.getRoot().getId());

        innerClass1 = et.addNode("InnerClass1", Node.Type.CLASS, class1.getId());

        method1 = et.addNode("method1", Node.Type.METHOD, class1.getId());
        method2 = et.addNode("method2", Node.Type.METHOD, class2.getId());
        method3 = et.addNode("method3", Node.Type.METHOD, innerClass1.getId());

        line1 = et.addNode("12", Node.Type.LINE, method1.getId());
        line2 = et.addNode("345", Node.Type.LINE, method2.getId());
        line3 = et.addNode("6789", Node.Type.LINE, method3.getId());
    }

    @Test
    public void testClassNodeNames () {
        assertEquals("package1.Class1", NodeUtils.getClassName(class1));
        assertEquals("package2.package3.Class2", NodeUtils.getClassName(class1));
        assertEquals("Class3", NodeUtils.getClassName(class1));
    }

    @Test
    public void testInnerClassNodeNames () {
        assertEquals("package1.Class1$InnerClass1", NodeUtils.getClassName(innerClass1));
    }

    @Test
    public void testMethodNodeNames () {
        assertEquals("method1", NodeUtils.getMethodName(method1));
        assertEquals("method2", NodeUtils.getMethodName(method2));
        assertEquals("method3", NodeUtils.getMethodName(method3));
    }

    @Test
    public void testMethodNodeClassNames () {
        assertEquals("package1.Class1", NodeUtils.getClassName(method1));
        assertEquals("package2.package3.Class2", NodeUtils.getClassName(method2));
        assertEquals("package1.Class1$InnerClass1", NodeUtils.getClassName(method3));
    }

    @Test
    public void testLineNode1 () {
        testLineNode(line1, "package1.Class1", "method1", 12);
    }

    @Test
    public void testLineNode2 () {
        testLineNode(line2, "package2.package3.Class2", "method2", 345);
    }

    @Test
    public void testLineNode3 () {
        testLineNode(line3, "package1.Class1$InnerClass1", "method3", 6789);
    }

    private void testLineNode (Node n,
                               String cls,
                               String method,
                               int number) {
        assertEquals(cls, NodeUtils.getClassName(n));
        assertEquals(method, NodeUtils.getMethodName(n));
        assertEquals(number, NodeUtils.getLineNumber(n));
    }
}