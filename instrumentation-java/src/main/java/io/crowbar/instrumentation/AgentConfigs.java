package io.crowbar.instrumentation;

import flexjson.JSON;
import flexjson.JSONDeserializer;
import flexjson.JSONSerializer;
import io.crowbar.instrumentation.events.MultiListener;
import io.crowbar.instrumentation.events.VerboseListener;
import io.crowbar.instrumentation.passes.FilterPass;
import io.crowbar.instrumentation.passes.InjectPass;
import io.crowbar.instrumentation.passes.InjectPass.Granularity;
import io.crowbar.instrumentation.passes.Pass;
import io.crowbar.instrumentation.passes.StackSizePass;
import io.crowbar.instrumentation.passes.TestWrapperPass;
import io.crowbar.instrumentation.passes.matchers.BlackList;
import io.crowbar.instrumentation.passes.matchers.ModifierMatcher;
import io.crowbar.instrumentation.passes.matchers.PrefixMatcher;
import io.crowbar.instrumentation.passes.wrappers.ActionTakerToTestWrapper;
import io.crowbar.instrumentation.passes.wrappers.JUnit3TestWrapper;
import io.crowbar.instrumentation.passes.wrappers.JUnit4TestWrapper;
import io.crowbar.instrumentation.passes.wrappers.TestNGTestWrapper;
import io.crowbar.instrumentation.runtime.Collector;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

import javassist.CtClass;
import javassist.Modifier;

public class AgentConfigs {
    private Granularity granularity = Granularity.FUNCTION;
    private int port = 1234;
    private List<String> blockPrefixes;
    private boolean debugDump = true;
    private boolean verbose = false;

    @JSON(include = false)
    private List<Pass> passes = new LinkedList<Pass> ();

    public List<Pass> getPasses () {
        return passes;
    }

    public void setGranularity (Granularity granularity) {
        this.granularity = granularity;
    }

    public Granularity getGranularity () {
        return granularity;
    }

    public void setPort (int port) {
        this.port = port;
    }

    public int getPort () {
        return port;
    }

    public List<String> getBlockPrefixes () {
        return blockPrefixes;
    }

    public boolean getDebugDump () {
        return debugDump;
    }

    public void setDebugDump (boolean debugDump) {
        this.debugDump = debugDump;
    }

    public boolean getVerbose () {
        return verbose;
    }

    public void setVerbose (boolean verbose) {
        this.verbose = verbose;
    }

    public void setBlockPrefixes (List<String> blockPrefixes) {
        this.blockPrefixes = blockPrefixes;
    }

    public AgentConfigs () {
        blockPrefixes = new ArrayList<String> ();
        Collections.addAll(blockPrefixes, "junit.",
                           "org.junit.",
                           "org.hamcrest",
                           "org.eclipse.",
                           "org.testng.",
                           "com.beust.");
    }

    // defaultconfigs
    public void configure () {
        if (debugDump) CtClass.debugDump = "debug";

        List<String> prefixes = new ArrayList<String> ();
        Collections.addAll(prefixes, "javax.",
                           "java.",
                           "sun.",
                           "javassist.",
                           "com.sun.",
                           "org.apache.",
                           "io.crowbar.");

        prefixes.addAll(blockPrefixes);

        // Ignores classes in particular packages
        PrefixMatcher pMatcher = new PrefixMatcher(prefixes);

        ModifierMatcher mMatcher = new ModifierMatcher(Modifier.NATIVE);

        FilterPass fp = new FilterPass(new BlackList(mMatcher),
                                       new BlackList(pMatcher));
        passes.add(fp);


        // Injects instrumentation instructions
        InjectPass inject = new InjectPass(granularity);
        passes.add(inject);


        // Wraps unit tests with instrumentation instrunctions
        TestWrapperPass twp = new TestWrapperPass(
            new ActionTakerToTestWrapper(
                new BlackList(
                    new ModifierMatcher(Modifier.ABSTRACT))), // Skip Abstract Methods
            new JUnit3TestWrapper(),
            new JUnit4TestWrapper(),
            new TestNGTestWrapper());
        passes.add(twp);

        // Recalculates the stack size for all methods
        StackSizePass stackSizePass = new StackSizePass();
        passes.add(stackSizePass);


        MultiListener ml = new MultiListener();
        VerboseListener vl = new VerboseListener();

        if (verbose)
            ml.add(vl);

        Client cl = new Client(null, port);
        ml.add(cl);

        Collector.start("Workspace-" + cl.getCliendId(), ml);
    }

    public String serialize () {
        return new JSONSerializer().exclude("*.class").deepSerialize(this);
    }

    public static AgentConfigs deserialize (String str) {
        try {
            return new JSONDeserializer<AgentConfigs> ().deserialize(str, AgentConfigs.class);
        }
        catch (Throwable t) {
            return null;
        }
    }
}