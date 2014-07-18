package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.messaging.Messages.Message;
import io.crowbar.instrumentation.runtime.CollectorListener;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.util.io.ThreadedServer;

import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class AgentServer extends ThreadedServer implements CollectorListener {

	BlockingQueue<Message> messages = new LinkedBlockingQueue<Message>();
	
	private class AgentService implements Runnable {

		@Override
		public void run() {
			Message message;
			while(true) { //TODO: socket connection
				while((message = messages.poll()) != null) {
					System.out.println("server " + message);
				}
			}
		}
    }
	
	public AgentServer(ServerSocket serverSocket) {
		super(serverSocket);
	}

	@Override
	protected Runnable handle(Socket s) {
		return new AgentService();
	}

	@Override
	public void startTransaction(Probe p) {
		messages.add(new Messages.TransactionStartMessage().setProbe(p));
	}

	@Override
	public void endTransaction(Probe p) {
		messages.add(new Messages.TransactionEndMessage().setProbe(p));
	}

}
