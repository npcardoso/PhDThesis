package io.crowbar.instrumentation.examples.testng;

public class ExceptionTest {
    @org.testng.annotations.Test(expectedExceptions = ArithmeticException.class)
    public void divisionWithException () {
        @SuppressWarnings("unused")
        int i = 1 / 0;
    }

    @org.testng.annotations.Test(expectedExceptions = Exception.class,
                                 expectedExceptionsMessageRegExp = "asd")
    public void expectedMessage () throws Exception {
        throw new Exception("asd");
    }
}