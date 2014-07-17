package io.crowbar.instrumentation.messaging;

import io.crowbar.instrumentation.runtime.*;
import java.net.Socket;


public interface InstrumentationProtocol {
    // These methods should send and receive all static information regarding classes (e.g., classid, classname, etc..). The probes should not be transmitted.
    public void sendClass (Socket s, Probe p); // throws something
    public Probe rcvClass (Socket s); // throws something


    // These methods should send and receive all static information regarding probes (e.g., classid, methodname, line, etc..).
    public void sendProbe (Socket s, Probe p); // throws something
    public Probe rcvProbe (Socket s); // throws something

    public void sendHitVector (Socket s, boolean[] hit_vector); // throws something
    public boolean[] rcvHitVector (Socket s); // throws something

    // ...
}