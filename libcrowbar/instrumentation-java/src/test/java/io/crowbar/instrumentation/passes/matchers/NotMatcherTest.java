package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.FalseMatcher;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.TrueMatcher;

import org.junit.Test;

public class NotMatcherTest {
    final private Matcher trueMatcher = new TrueMatcher();
    final private Matcher falseMatcher = new FalseMatcher();

    @Test
    public void testClassMatcher () {
        assertEquals(new NotMatcher(trueMatcher).matches(null), false);
        assertEquals(new NotMatcher(falseMatcher).matches(null), true);
    }

    @Test
    public void testMethodMatcher () {
        assertEquals(new NotMatcher(trueMatcher).matches(null, null), false);
        assertEquals(new NotMatcher(falseMatcher).matches(null, null), true);
    }
}