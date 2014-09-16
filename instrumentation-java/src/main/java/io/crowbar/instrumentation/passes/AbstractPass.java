package io.crowbar.instrumentation.passes;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;
import javassist.CtClass;
import javassist.CtMethod;
import java.util.StringTokenizer;


public abstract class AbstractPass implements Pass {
    private Node getNode (Collector c,
                          Node parent,
                          String name,
                          Node.Type type) {
        Node tmp = parent.getChild(name);


        if (tmp == null)
            tmp = c.registerNode(name, type, parent.getId());


        assert (tmp.getType() == type);

        return tmp;
    }

    protected final Node getNode (CtClass cls) {
        Collector c = Collector.instance();
        Node n = c.getRootNode();
        String tok = cls.getName();


        // Extract Package Hierarchy
        int pkgEnd = tok.lastIndexOf(".");


        if (pkgEnd >= 0) {
            StringTokenizer stok = new StringTokenizer(tok.substring(0, pkgEnd), ".");

            while (stok.hasMoreTokens()) {
                n = getNode(c, n, stok.nextToken(), Node.Type.PACKAGE);
            }
        } else
            pkgEnd = -1;


        // Extract Class Hierarchy
        StringTokenizer stok = new StringTokenizer(tok.substring(pkgEnd + 1), "$");

        while (stok.hasMoreTokens()) {
            tok = stok.nextToken();
            n = getNode(c, n, tok, Node.Type.CLASS);
        }


        return n;
    }

    protected final Node getNode (CtClass cls,
                                  CtMethod m) {
        Collector c = Collector.instance();
        Node parent = getNode(cls);


        return getNode(c, parent, m.getName(), Node.Type.METHOD);
    }

    protected final Node getNode (CtClass cls,
                                  CtMethod m,
                                  int line) {
        Collector c = Collector.instance();
        Node parent = getNode(cls, m);


        return getNode(c, parent, "" + line, Node.Type.LINE);
    }

    protected final HitProbe registerProbe (CtClass cls,
                                            Node n,
                                            ProbeType type) {
        Collector c = Collector.instance();


        return c.registerProbe(cls.getName(), n.getId(), type);
    }
}