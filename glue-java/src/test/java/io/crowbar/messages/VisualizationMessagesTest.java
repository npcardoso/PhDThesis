package io.crowbar.messages;

import static org.junit.Assert.*;
import flexjson.JSONDeserializer;
import flexjson.JSONSerializer;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.DiagnosticSystemFactory;
import io.crowbar.diagnostic.spectrum.EditableTree;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;
import io.crowbar.messages.DiagnosticMessages.Request;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;
import java.util.Scanner;

import org.junit.Test;

public class VisualizationMessagesTest {
    static List<Double> randomizeScores (int numNodes) {
        List<Double> l = new ArrayList<Double> ();
        Random r = new Random();

        while (numNodes-- > 0) {
            l.add(r.nextDouble());
        }

        return l;
    }

    static Tree randomizeTree (int numNodes) {
        EditableTree t = new EditableTree("asd");
        Random r = new Random();


        while (numNodes-- > 0) {
            t.addNode("" + r.nextInt(), r.nextInt(t.size()));
        }

        return t;
    }

    @Test
    public void testVisualizationMessages () {
        // needs to be improverd.
        List<Double> lscores = new ArrayList<Double> (Arrays.asList(0.3624116251182534, 0.23679368533093226, 0.1716916465415933));

        Tree t = randomizeTree(3);

        VisualizationMessages.Request v = VisualizationMessages.issueRequest(t, lscores);

        String jsonRequest = Messages.serialize(v);

        JSONDeserializer<io.crowbar.messages.Response> jsreq = Messages.getResponseDeserializer();
        DiagnosticMessages.Response rqst = (DiagnosticMessages.Response)jsreq.deserialize(jsonRequest);

        assertTrue(jsonRequest.startsWith("{\"scores\":[0.3624116251182534,0.23679368533093226,0.1716916465415933],\"tree\""));
    }
}