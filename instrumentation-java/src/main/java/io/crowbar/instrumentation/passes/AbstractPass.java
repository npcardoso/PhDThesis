package io.crowbar.instrumentation.passes;

import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;

import javassist.CtClass;
import javassist.CtMethod;


public abstract class AbstractPass implements Pass {
    protected final Node getNode (CtClass cls) {
        Collector c = Collector.instance();
        Node root = c.getRootNode();
        Node n = root.getChild(cls.getName());


        if (n == null)
            n = c.registerNode(cls.getName(),
                               root.getId());

        return n;
    }

    protected final Node getNode (CtClass cls,
                                  CtMethod m) {
        Collector c = Collector.instance();
        Node parent = getNode(cls);
        Node n = parent.getChild(m.getName());


        if (n == null)
            n = c.registerNode(m.getName(),
                               parent.getId());

        return n;
    }

    protected final Node getNode (CtClass cls,
                                  CtMethod m,
                                  int line) {
        Collector c = Collector.instance();
        Node parent = getNode(cls, m);
        Node n = parent.getChild("" + line);


        if (n == null)
            n = c.registerNode("" + line, parent.getId());

        return n;
    }

    protected final HitProbe registerProbe (CtClass cls,
                                            Node n,
                                            ProbeType type) {
        Collector c = Collector.instance();


        return c.registerProbe(cls.getName(), n.getId(), type);
    }
}

malabar finished at Fri Aug 22 14:13:16
