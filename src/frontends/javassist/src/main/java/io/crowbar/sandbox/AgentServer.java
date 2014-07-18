package io.crowbar.sandbox;

import io.crowbar.instrumentation.messaging.Messages.*;
import io.crowbar.instrumentation.messaging.Server;
import io.crowbar.instrumentation.passes.IgnorePass;
import io.crowbar.instrumentation.passes.InjectPass;
import io.crowbar.instrumentation.passes.Pass;
import io.crowbar.instrumentation.passes.TestWrapperPass;
import io.crowbar.instrumentation.passes.wrappers.JUnit4Wrapper;
import io.crowbar.instrumentation.runtime.VerboseCollectorListener;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.ProbeSet;

import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.ProtectionDomain;
import java.util.LinkedList;
import java.util.List;

import javassist.ClassPool;
import javassist.CtClass;
import javassist.Modifier;

public class AgentServer {
    public static class VerboseService extends Server.Service {
        public VerboseService (Socket s) {
            super(s);
        }

        @Override
        protected void handle (TransactionStartMessage m) throws Exception {
            System.out.println("Received TransactionStartMessage: " + m);
        }

        @Override
        protected void handle (TransactionEndMessage m) throws Exception {
            System.out.println("Received TransactionEndMessage: " + m);

            for (boolean b : m.hit_vector)
                System.out.print(b ? "1 " : "0 ");

            System.out.println("");
        }

        @Override
        protected void handle (OracleMessage m) throws Exception {
            System.out.println("Received OracleMessage: " + m);
        }
    }

    public static class VerboseServiceFactory extends Server.ServiceFactory {
        @Override
        public Server.Service create (Socket s) {
            return new VerboseService(s);
        }
    }


    public static void main (String[] args) {
        try {
            Server s = new Server(new VerboseServiceFactory(),
                                  new ServerSocket(1234));
            s.start();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}