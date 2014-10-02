package io.crowbar.instrumentation.examples.junit;

import org.junit.Ignore;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.Timeout;

@Ignore
public class TimeoutTest {
    @Rule
    public Timeout globalTimeout = new Timeout(10000);

    @Test(timeout = 1000)
    public void testWithLocalTimeout () throws InterruptedException {
        Thread.sleep(1000);
    }

    @Test
    public void testWithGlobalTimeout () throws InterruptedException {
        Thread.sleep(9999);
    }
}