package io.crowbar.instrumentation.examples.junit;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;

@RunWith(Suite.class)
@Suite.SuiteClasses({
                        AssertTest.class,
                        AssumeTest.class
                    })
public class SuiteTest {
    // empty
}