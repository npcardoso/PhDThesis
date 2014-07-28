package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import javassist.CtClass;
import javassist.CtMethod;

public class FilterPass extends AbstractPass {
    private final List<ActionTaker> actionTakers = new LinkedList<ActionTaker> ();
    private Outcome fallbackOutcome = Outcome.CONTINUE;
    private Outcome acceptOutcome = Outcome.CONTINUE;
    private Outcome rejectOutcome = Outcome.ABORT;


    public FilterPass (ActionTaker... actionTakers) {
        this.actionTakers.addAll(Arrays.asList(actionTakers));
    }

    public FilterPass (List<ActionTaker> actionTakers) {
        this.actionTakers.addAll(actionTakers);
    }

    public final FilterPass setFallbackOutcome (Outcome targetOutcome) {
        fallbackOutcome = targetOutcome;
        return this;
    }

    public final FilterPass setAcceptOutcome (Outcome targetOutcome) {
        acceptOutcome = targetOutcome;
        return this;
    }

    public final FilterPass setReject (Outcome targetOutcome) {
        rejectOutcome = targetOutcome;
        return this;
    }

    @Override
    public Outcome transform (CtClass c) throws Exception {
        for (ActionTaker at : actionTakers) {
            ActionTaker.Action ret = at.getAction(c);

            switch (ret) {
            case ACCEPT:
                return acceptOutcome;

            case NEXT:
                continue;

            case REJECT:
                return rejectOutcome;
            }
        }

        return fallbackOutcome;
    }
}