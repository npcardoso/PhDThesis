package io.crowbar.instrumentation.passes;

import java.util.*;
import javassist.*;

public class IgnorePass extends Pass {

    @Override
    public void transform(CtClass c) throws Exception {
        for(String s : ignores) {
            if(c.getName().startsWith(s)) {
                System.out.println("Ignoring Class '" + c.getName() + "' due to ignore rule '" + s + "'");
                throw new IgnoreClassException();
            }
        }
    }

    public List<String> ignores = new LinkedList<String>();
}
