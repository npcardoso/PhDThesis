package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.HitVector;
import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.runtime.Tree.Node;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import javassist.CtClass;
import javassist.CtMethod;


public abstract class AbstractPass implements Pass {
    protected Node getNode (CtClass c) throws RegistrationException {
        Node root = Collector.getDefault().getTree().getRoot();
        Node n = root.getChild(c.getName());


        if (n == null)
            n = root.addChild(c.getName());

        return n;
    }

    protected Node getNode (CtClass c,
                            CtMethod m) throws RegistrationException {
        Node parent = getNode(c);
        Node n = parent.getChild(m.getName());


        if (n == null)
            n = parent.addChild(m.getName());

        return n;
    }

    protected Node getNode (CtClass c,
                            CtMethod m,
                            int line) throws RegistrationException {
        Node parent = getNode(c, m);
        Node n = parent.getChild("" + line);


        if (n == null)
            n = parent.addChild("" + line);

        return n;
    }

    protected Probe registerProbe (CtClass c,
                                   Node n,
                                   ProbeType type) throws RegistrationException {
        HitVector hv = Collector.getDefault().getHitVector();


        return hv.registerProbe(c.getName(), n.getId(), type);
    }
}