package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;

import java.util.Set;

import javassist.CtClass;
import javassist.CtMethod;

public interface TestWrapper extends ActionTaker {
    Set<String> validExceptions (CtClass c,
                                 CtMethod m);
}