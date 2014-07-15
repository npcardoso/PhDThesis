package io.crowbar.instrumentation;

import io.crowbar.instrumentation.runtime.*;

import static org.junit.Assert.*;
import org.junit.Test;

import javassist.*;

public class App
{


    private class brogle {
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

	@Test
	public void discoversExpiredCreditCard() {
            assertEquals(1,0);
        }

    }

    public static void main( String[] args ) {
        Collector.getDefault().hitprobe(543);
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


    private int crasps () {
        return 9999;
    }

}
