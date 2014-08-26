package io.crowbar.diagnostic.runners;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import flexjson.JSONSerializer;
import io.crowbar.diagnostic.Connection;
import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.DiagnosticSystemFactory;
import io.crowbar.diagnostic.SortedDiagnostic;
import io.crowbar.diagnostic.algorithms.FuzzinelRanker;
import io.crowbar.diagnostic.algorithms.MHSGenerator;
import io.crowbar.diagnostic.algorithms.SimilarityRanker;
import io.crowbar.diagnostic.algorithms.SingleFaultGenerator;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

import org.junit.Test;

public class JNARunnerTest {
    @Test
    public void testJNARunnerSUM () {
        String in = "3 3 1 0 1 1.0 1 1 0 1.0 1 1 1 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new SingleFaultGenerator());

        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        Connection ochiaiCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(ochiaiCon);


            System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));

            List<Double> scores = s.getScorePerProbe(diag, Spectrum.SUM);


            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.816497, 0.5, 0.5));


            assertEquals(scores.size(), cmp.size());
            assertEquals(scores, cmp);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testJNARunnerAVG () {
        String in = "3 3 1 0 1 1.0 1 1 0 1.0 1 1 1 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new SingleFaultGenerator());

        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        Connection ochiaiCon = j.addConnection(0, 0);

        System.out.println(ochiaiCon.toString()); // this way, class will be fully tested -- Hack

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(ochiaiCon);


            System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));

            List<Double> scores = s.getScorePerProbe(diag, Spectrum.AVG);


            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.816497, 0.5, 0.5));


            assertEquals(scores.size(), cmp.size());
            assertEquals(scores, cmp);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testJNARunnerMAX () {
        String in = "3 3 1 0 1 1.0 1 1 0 1.0 1 1 1 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new SingleFaultGenerator());

        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        Connection ochiaiCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(ochiaiCon);


            System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));

            List<Double> scores = s.getScorePerProbe(diag, Spectrum.MAX);


            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.816497, 0.5, 0.5));


            assertEquals(scores.size(), cmp.size());
            assertEquals(scores, cmp);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testJNARunnerBigAVG () {
        String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new SingleFaultGenerator());

        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        Connection ochiaiCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(ochiaiCon);


            // System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));

            List<Double> scores = s.getScorePerProbe(diag, Spectrum.AVG);

            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.5));


            assertEquals(scores.size(), cmp.size());
            assertEquals(scores, cmp);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testJNARunnerBigMAX () {
        String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new SingleFaultGenerator());

        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        Connection ochiaiCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(ochiaiCon);


            List<Double> scores = s.getScorePerProbe(diag, Spectrum.MAX);

            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.5));


            assertEquals(scores.size(), cmp.size());
            assertEquals(scores, cmp);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testJNARunnerBigSUM () {
        String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new SingleFaultGenerator());

        j.addRanker(new SimilarityRanker(SimilarityRanker.Type.OCHIAI));
        Connection ochiaiCon = j.addConnection(0, 0);


        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(ochiaiCon);


            List<Double> scores = s.getScorePerProbe(diag, Spectrum.SUM);

            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.5));


            assertEquals(scores.size(), cmp.size());
            assertEquals(scores, cmp);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testJNARunnerAVG_MHS_FUZ () {
        String in = "3 3 1 0 1 1.0 0 1 0 1.0 1 1 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new MHSGenerator());

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(fuzzinelCon);


            System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));

            List<Double> scores = s.getScorePerProbe(diag, Spectrum.AVG);

            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.333333, 0.5, 0.666667));

            assertEquals(scores, cmp);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testJNARunnerSUM_MHS_FUZ () {
        String in = "3 3 1 0 1 1.0 0 1 0 1.0 1 1 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new MHSGenerator());

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(fuzzinelCon);


            System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));

            // FIXME: May use getScorePerProbe
            List<Double> scores = s.getScorePerProbe(diag, Spectrum.SUM);


            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.333333, 1.0, 0.666667));

            assertEquals(scores, cmp);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testJNARunnerMAX_MHS_FUZ () {
        String in = "3 3 1 0 1 1.0 0 1 0 1.0 1 1 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        MHSGenerator mhs = new MHSGenerator();
        mhs.setLambda(1);
        j.addGenerator(mhs);

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(fuzzinelCon);


            System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));

            List<Double> scores = s.getScorePerProbe(diag, Spectrum.MAX);

            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.333333, 0.666667, 0.666667));

            assertEquals(scores, cmp);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testJNARunnerMAX_MHS_FUZLambda () {
        String in = "3 3 1 0 1 1.0 0 1 0 1.0 1 1 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        MHSGenerator mhs = new MHSGenerator();
        mhs.setLambda(1);
        mhs.setMaxCandidates(10);
        mhs.setMaxDepth(10);
        mhs.setMaxTime(1000);
        mhs.setMinScore(1);
        mhs.setHeuristic(SimilarityRanker.Type.OCHIAI);
        j.addGenerator(mhs);

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(fuzzinelCon);


            System.out.println(new JSONSerializer().exclude("*.class").deepSerialize(dr));

            List<Double> scores = s.getScorePerProbe(diag, Spectrum.MAX);

            List<Double> cmp = new ArrayList<Double> (Arrays.asList(0.333333, 0.666667, 0.666667));

            assertEquals(scores, cmp);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test(expected = IndexOutOfBoundsException.class)
    public void testJDiagnosticSystemFactory_ConnectionFromNegative () {
        String in = "3 3 1 0 1 1.0 0 1 0 1.0 1 1 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new MHSGenerator());

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(-1, 0);
    }

    @Test(expected = IndexOutOfBoundsException.class)
    public void testJDiagnosticSystemFactory_ConnectionFromInexistent () {
        String in = "3 3 1 0 1 1.0 0 1 0 1.0 1 1 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new MHSGenerator());

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(10, 0);
    }

    @Test(expected = IndexOutOfBoundsException.class)
    public void testJDiagnosticSystemFactory_ConnectionToInexistent () {
        String in = "3 3 1 0 1 1.0 0 1 0 1.0 1 1 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new MHSGenerator());

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(0, 10);
    }

    @Test(expected = IndexOutOfBoundsException.class)
    public void testJDiagnosticSystemFactory_ConnectionToNegative () {
        String in = "3 3 1 0 1 1.0 0 1 0 1.0 1 1 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new MHSGenerator());

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(0, -1);
    }

    @Test
    public void testJNARunner_SortedDiagnostic () {
        String in = "3 3 1 0 1 1.0 0 1 0 1.0 1 1 0 0.0";


        Spectrum<Hit, ? > s = HitSpectrumUnserializer.unserialize(new Scanner(in));

        DiagnosticSystemFactory j = new DiagnosticSystemFactory();

        j.addGenerator(new MHSGenerator());

        j.addRanker(new FuzzinelRanker());
        Connection fuzzinelCon = j.addConnection(0, 0);

        try {
            JNARunner runner = new JNARunner();

            DiagnosticReport dr = runner.run(j.create(), s);

            Diagnostic diag = dr.getDiagnostic(fuzzinelCon);

            SortedDiagnostic sdiag = new SortedDiagnostic(diag);

            List<Double> scores = s.getScorePerNode(diag, Spectrum.SUM);

            List<Double> cmp = new ArrayList<Double> (Arrays.asList(1.0, 0.666667, 0.333333));

            assertEquals(sdiag.size(), scores.size());
            assertEquals(sdiag.get(0).getScore(), 1.0, 0.01);
            assertEquals(sdiag.getSortedDiagnostic(), cmp);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}