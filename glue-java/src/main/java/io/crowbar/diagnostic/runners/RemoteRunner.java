package io.crowbar.diagnostic.runners;

import java.io.IOException;
import java.net.Socket;


public class RemoteRunner extends StreamRunner {
    public RemoteRunner (Socket s) throws IOException {
        super(s.getInputStream(), s.getOutputStream());
    }
}
