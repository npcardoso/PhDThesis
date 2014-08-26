package io.crowbar.diagnostic.runners;

import flexjson.JSONSerializer;
import io.crowbar.diagnostic.Connection;
import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.DiagnosticSystemFactory;
import io.crowbar.diagnostic.algorithms.Ranker;
import io.crowbar.diagnostic.algorithms.SimilarityRanker;
import io.crowbar.diagnostic.algorithms.SingleFaultGenerator;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;
import io.crowbar.messages.VisualizationMessages;

import java.util.List;
import java.util.Scanner;

import org.junit.Test;

public class JNARunnerTest {
    @Test
    public void testJNARunner () {
        String in = "3 3 1 0 1 1.0 1 1 0 1.0 1 1 1 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new SingleFaultGenerator());
        // j.addGenerator(new MHSGenerator());

        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        Connection ochiaiCon = j.addConnection(0, 0);
        // j.addConnection(1, 0);

        // j.addRanker(new FuzzinelRanker());
        // Connection fuzzinelCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(ochiaiCon);


            System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));


            // FIXME: May use getScorePerProbe
            List<Double> scores = s.getScorePerNode(diag, Spectrum.SUM);

            // FIXME: this does not make sense.. the unserialized spectra will not have a proper tree
            Tree t = s.getTree();


            String jsonRequest = io.crowbar.messages.Messages.serialize(
                VisualizationMessages.issueRequest(t, scores));
            System.out.println(jsonRequest);
            // TODO: conclude
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }
}