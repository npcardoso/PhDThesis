package io.crowbar.instrumentation;

import io.crowbar.instrumentation.runtime.*;
import io.crowbar.util.io.*;

import static org.junit.Assert.*;
import org.junit.Test;
import java.net.ServerSocket;

import javassist.*;

public class App
{

    public static boolean[] ASDsdaf = new boolean[1235];

    private static class brogle {
        private int trackles (int a, float b) {
            if(a>b)
                return a;
            else
                for(;a<b;a++){
                    if(a + "" == "123")
                        return a;
                }
            return (int)b;
        }

        private static void broglestar(){
            ASDsdaf[444] = true;
        }

	@Test
	public void discoversExpiredCreditCard() {
            assertEquals(1,0);
        }

    }

    public static void main( String[] args ) {
        try {
            ThreadedServer server = new EchoServer(new ServerSocket(1234));
            server.max_clients = 2;
            server.start();
            server.join();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
//Collector.getDefault().hitprobe(543);
        brogle b = new brogle();
        b.discoversExpiredCreditCard();
    }

    private int blurz(int a, float b) {
        if(a>b)
            return a;
        else
            for(;a<b;a++){
                if(a + "" == "123")
                    return a;
            }
        return (int)b;
    }

    private void tracklerbar () {
        ASDsdaf[444] = true;
    }

    private int crasps () {
        return 9999;
    }

}
