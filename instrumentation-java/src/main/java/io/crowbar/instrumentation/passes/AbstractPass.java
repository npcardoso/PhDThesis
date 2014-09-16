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
                          String name) {
        Node tmp = parent.getChild(name);


        if (tmp == null)
            tmp = c.registerNode(name, parent.getId());

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
                n = getNode(c, n, stok.nextToken());
            }
        } else
            pkgEnd = -1;


        // Extract Class Hierarchy
        StringTokenizer stok = new StringTokenizer(tok.substring(pkgEnd + 1), "$");

        while (stok.hasMoreTokens()) {
            tok = stok.nextToken();
            n = getNode(c, n, tok);
        }


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