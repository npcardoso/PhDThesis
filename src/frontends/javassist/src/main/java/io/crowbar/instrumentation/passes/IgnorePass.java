package io.crowbar.instrumentation.passes;

import java.util.*;
import javassist.*;

public class IgnorePass extends Pass {

    public IgnorePass (boolean blacklist) {
        this.blacklist = blacklist;
    }

    @Override
    public void transform(CtClass c) throws Exception {
        for(String s : prefix) {
            if(c.getName().startsWith(s)) {
                if(blacklist)
                    throw new IgnoreClassException();
                else
                    return;
            }
        }
        for(String s : suffix) {
            if(c.getName().endsWith(s)) {
                if(blacklist)
                    throw new IgnoreClassException();
                else
                    return;
            }
        }

        if(!blacklist)
            throw new IgnoreClassException();
    }

    private boolean blacklist;
    public List<String> prefix = new LinkedList<String>();
    public List<String> suffix = new LinkedList<String>();

}
