package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;

import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;

public class ArgumentTypeMatcher implements Matcher {
    private static final Logger logger = LogManager.getLogger(ArgumentTypeMatcher.class);

    private final CtClass[] types;

    public ArgumentTypeMatcher (CtClass[] types) {
        this.types = types;
    }

    public ArgumentTypeMatcher () {
        this(new CtClass[] {});
    }

    @Override
    public final boolean matches (CtClass c) {
        return false;
    }

    @Override
    public final boolean matches (CtClass c,
                                  CtMethod m) {
        try {
            CtClass[] methodArgs = m.getParameterTypes();

            if (types.length != methodArgs.length)
                return false;

            for (int i = 0; i < types.length; i++) {
                if (!types[i].equals(methodArgs[i]))
                    return false;
            }
        }
        catch (Exception e) {
            logger.error(e, e);
            return false;
        }

        return true;
    }
}