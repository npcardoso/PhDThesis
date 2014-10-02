package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.FalseMatcher;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.TrueMatcher;

import org.junit.Test;

public class AndMatcherTest {
    final private Matcher trueMatcher = new TrueMatcher();
    final private Matcher falseMatcher = new FalseMatcher();

    @Test
    public void testClassMatcher () {
        assertEquals(new AndMatcher(trueMatcher, trueMatcher).matches(null), true);
        assertEquals(new AndMatcher(trueMatcher, falseMatcher).matches(null), false);
        assertEquals(new AndMatcher(falseMatcher, trueMatcher).matches(null), false);
        assertEquals(new AndMatcher(falseMatcher, falseMatcher).matches(null), false);
    }

    @Test
    public void testMethodMatcher () {
        assertEquals(new AndMatcher(trueMatcher, trueMatcher).matches(null, null), true);
        assertEquals(new AndMatcher(trueMatcher, falseMatcher).matches(null, null), false);
        assertEquals(new AndMatcher(falseMatcher, trueMatcher).matches(null, null), false);
        assertEquals(new AndMatcher(falseMatcher, falseMatcher).matches(null, null), false);
    }
}