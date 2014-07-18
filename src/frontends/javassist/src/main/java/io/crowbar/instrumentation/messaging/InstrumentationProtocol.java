package io.crowbar.instrumentation.messaging;


import io.crowbar.instrumentation.runtime.*;
import java.net.InputStream;


public interface InstrumentationProtocol {
    // These methods should send and receive all static information regarding classes (e.g., classid, classname, etc..). The probes should not be transmitted.
    public void sendProbeSet (OutputStream s, ProbeSet p); // throws something
    public ProbeSet rcvProbeSet (InputStream s); // throws something


    // These methods should send and receive all static information regarding probes (e.g., classid, methodname, line, etc..).
    public void sendProbe (OutputStream s, Probe p); // throws something
    public Probe rcvProbe (InputStream s); // throws something

    public void sendHitVector (OutputStream s, boolean[] hit_vector); // throws something
    public boolean[] rcvHitVector (InputStream s); // throws something

    // ...
}