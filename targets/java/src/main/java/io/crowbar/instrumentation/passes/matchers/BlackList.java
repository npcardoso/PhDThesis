package io.crowbar.instrumentation.passes.matchers;

public class BlackList extends AbstractActionTaker {
    public BlackList (Matcher m) {
        super(m);
    }

    @Override
    public Action getAction (boolean matches) {
        if (matches)
            return Action.REJECT;

        return Action.NEXT;
    }
}