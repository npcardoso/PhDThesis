package io.crowbar.instrumentation.passes.matchers;

import static org.junit.Assert.*;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.FalseMatcher;
import io.crowbar.instrumentation.passes.matchers.MatcherUtils.TrueMatcher;

import org.junit.Test;

public class OrMatcherTest {
    final private Matcher trueMatcher = new TrueMatcher();
    final private Matcher falseMatcher = new FalseMatcher();

    @Test
    public void testClassMatcher () {
        assertEquals(new OrMatcher(trueMatcher, trueMatcher).matches(null), true);
        assertEquals(new OrMatcher(trueMatcher, falseMatcher).matches(null), true);
        assertEquals(new OrMatcher(falseMatcher, trueMatcher).matches(null), true);
        assertEquals(new OrMatcher(falseMatcher, falseMatcher).matches(null), false);
    }

    @Test
    public void testMethodMatcher () {
        assertEquals(new OrMatcher(trueMatcher, trueMatcher).matches(null, null), true);
        assertEquals(new OrMatcher(trueMatcher, falseMatcher).matches(null, null), true);
        assertEquals(new OrMatcher(falseMatcher, trueMatcher).matches(null, null), true);
        assertEquals(new OrMatcher(falseMatcher, falseMatcher).matches(null, null), false);
    }
}