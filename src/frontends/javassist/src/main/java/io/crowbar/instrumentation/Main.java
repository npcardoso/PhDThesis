package io.crowbar.instrumentation;

import java.lang.instrument.Instrumentation;
import javassist.*;

public class Main {
    public static void premain(String agentArgs, Instrumentation inst) {
        CtClass.debugDump = "debug";

        Agent a = new Agent();
        a.addPass(new InjectStatementPass());
        a.addPass(new PrintByteCodePass());
//        a.addPass(new WrapJunit4());

        inst.addTransformer(a);
    }

}
