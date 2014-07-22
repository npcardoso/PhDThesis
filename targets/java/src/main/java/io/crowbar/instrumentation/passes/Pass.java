package io.crowbar.instrumentation.passes;

import io.crowbar.sandbox.Tree;
import io.crowbar.sandbox.Tree.Node;


import io.crowbar.instrumentation.runtime.*;

import java.util.logging.Logger;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;


public abstract class Pass {
    public static class IgnoreClassException extends Exception {};

    public abstract void transform (CtClass c) throws Exception;

    protected Node getNode (CtClass c) {
        Node root = Collector.getDefault().getTree().getRoot();
        Node n = root.getChild(c.getName());


        if (n == null)
            n = root.addChild(c.getName());

        return n;
    }

    protected Node getNode (CtClass c,
                            CtMethod m) {
        Node parent = getNode(c);
        Node n = parent.getChild(m.getName());


        if (n == null)
            n = parent.addChild(m.getName());

        return n;
    }

    protected Node getNode (CtClass c,
                            CtMethod m,
                            int line) {
        Node parent = getNode(c, m);
        Node n = parent.getChild("" + line);


        if (n == null)
            n = parent.addChild("" + line);

        return n;
    }

    protected int registerProbe (CtClass c,
                                 Node n,
                                 ProbeType type) {
        System.out.println("Registering: " + n);
        return 0; // TODO
    }
}