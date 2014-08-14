package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import javassist.CtClass;
import javassist.CtMethod;


public abstract class AbstractPass implements Pass {
    protected final Node getNode (CtClass cls) throws RegistrationException {
        Collector c = Collector.instance();
        Node root = c.getRootNode();
        Node n = root.getChild(cls.getName());


        if (n == null)
            n = c.addNode(cls.getName(),
                          root);

        return n;
    }

    protected final Node getNode (CtClass cls,
                                  CtMethod m) throws RegistrationException {
        Collector c = Collector.instance();
        Node parent = getNode(cls);
        Node n = parent.getChild(m.getName());


        if (n == null)
            n = c.addNode(m.getName(), parent);

        return n;
    }

    protected final Node getNode (CtClass cls,
                                  CtMethod m,
                                  int line) throws RegistrationException {
        Collector c = Collector.instance();
        Node parent = getNode(cls, m);
        Node n = parent.getChild("" + line);


        if (n == null)
            n = c.addNode("" + line, parent);

        return n;
    }

    protected final HitProbe registerProbe (CtClass cls,
                                            Node n,
                                            ProbeType type) throws RegistrationException {
        Collector c = Collector.instance();


        return c.registerProbe(cls.getName(), n.getId(), type);
    }
}