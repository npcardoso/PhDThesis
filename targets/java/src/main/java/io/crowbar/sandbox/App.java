package io.crowbar.sandbox;

import io.crowbar.util.io.ThreadedServer;

import static org.junit.Assert.assertEquals;
import org.junit.Assume;

import java.util.ArrayList;

public class App {
    private static boolean[] aSDsdaf = new boolean[1235];

    private static class Brogle {
        private int trackles (int a, float b) {
            if (a > b)
                return a;
            else
                for (; a < b; a++) {
                    if (a + "" == "123")
                        return a;
                }

            return (int) b;
        }

        private static void broglestar (final Brogle asd) {
            aSDsdaf[444] = true;
        }

        @org.testng.annotations.Test()
        public void emptyTestNG () { // !Should Pass
        }

        @org.testng.annotations.Test(expectedExceptions = ArithmeticException.class)
        public void divisionWithException () { // !Should Pass
            int i = 1 / 0;
        }

        @org.testng.annotations.Test(expectedExceptions = Exception.class,
                                     expectedExceptionsMessageRegExp = "asd")
        public void expectedMessage () throws Exception { // !Should pass
            throw new Exception("asd");
        }

        @org.testng.annotations.Test(expectedExceptions = Exception.class,
                                     expectedExceptionsMessageRegExp = "asd")
        public void expectedMessage2 () throws Exception { // !Should fail
            throw new Exception("asda");
        }

        @org.junit.Test
        public void discoversExpiredCreditCard () { // !Should Pass
            trackles(1, 4);
            assertEquals(1, 1);
        }

        @org.junit.Test
        public void discoversExpiredCreditCard2 () { // !Should Fail
            assertEquals(1, 0);
        }

        @org.junit.Test
        public void testAssume () { // !Should Pass
            Assume.assumeTrue(1 == 0);
            assertEquals(1, 0);
        }

        @org.junit.Test(expected = IndexOutOfBoundsException.class) // !Should Pass
        public void testIndexOutOfBoundsException () {
            ArrayList emptyList = new ArrayList();
            Object o = emptyList.get(0);
        }

        @org.junit.Test(expected = Exception.class) // !Should Pass
        public void testExpectedSubclass () {
            ArrayList emptyList = new ArrayList();
            Object o = emptyList.get(0);
        }

        @org.junit.Test(expected = ArithmeticException.class) // !Should Fail
        public void testExpectedSubclass2 () {
            ArrayList emptyList = new ArrayList();
            Object o = emptyList.get(0);
        }
    }

    public static void main (String[] args) {
        // Collector.getDefault().hitprobe(543);
        Brogle b = new Brogle();


        // try {b.divisionWithException();} catch (Exception e) {}
        try {b.emptyTestNG();} catch (Exception e) {}
        try {b.divisionWithException();} catch (Exception e) {}
        try {b.expectedMessage();} catch (Exception e) {}
        try {b.expectedMessage2();} catch (Exception e) {}
        try {b.discoversExpiredCreditCard();} catch (Exception e) {}
        try {b.testAssume();} catch (Exception e) {}
        try {b.testIndexOutOfBoundsException();} catch (Throwable e) {}
        try {b.testExpectedSubclass();} catch (Throwable e) {}
        try {b.testExpectedSubclass2();} catch (Throwable e) {}
        try {b.discoversExpiredCreditCard2();} catch (Throwable e) {}


        /*try {
         *  ThreadedServer server = new EchoServer2(new ServerSocket(1234));
         *  server.max_clients = 2;
         *  server.start();
         *  server.join();
         * }
         * catch (Exception e) {
         *  e.printStackTrace();
         * }*/
    }

    private int blurz (int a, float b) {
        if (a > b)
            return a;
        else
            for (; a < b; a++) {
                if (a + "" == "123")
                    return a;
            }

        return (int) b;
    }

    private void tracklerbar () {
        aSDsdaf[444] = true;
    }

    private int crasps () {
        return 9999;
    }
}