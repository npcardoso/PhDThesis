package io.crowbar.instrumentation.spectrum;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;

import org.junit.Test;

public class SpectrumBuilderTest {
    @Test
    public void testEmptySpectrum () {
        SpectrumBuilder sb = new SpectrumBuilder();


        Spectrum spectrum = sb.getSpectrum();


        assertEquals(0, spectrum.getProbeCount());
        assertEquals(0, spectrum.getTransactionCount());
    }

    @Test
    public void testRegisterNode () throws Exception {
        SpectrumBuilder sb = new SpectrumBuilder();

        String new_node_name = "other node";


        sb.registerNode(1, 0, new_node_name, null);

        assertEquals(2, sb.getSpectrum().getTree().getNodes().size());

        Node root = sb.getSpectrum().getTree().getNodes().get(0);
        assertEquals(1, root.getChildren().size());

        Node new_node = sb.getSpectrum().getTree().getNodes().get(1);
        assertEquals(0, new_node.getChildren().size());
        assertEquals(new_node_name, new_node.getName());
    }

    @Test(expected = AssertionError.class)
    public void testRegisterProbe () throws Exception {
        SpectrumBuilder sb = new SpectrumBuilder();


        sb.registerProbe(0, 0, null);
    }

    @Test
    public void testEndTransaction () throws Exception {
        SpectrumBuilder sb = new SpectrumBuilder();


        sb.startTransaction(1);

        sb.registerNode(1, 0, "other node", null);
        sb.registerProbe(0, 1, ProbeType.TRANSACTION_START);
        sb.endTransaction(3, new boolean[] {false});

        assertEquals(1, sb.getSpectrum().getTransactionCount());

        Transaction tr = sb.getSpectrum().getTransaction(0);
        assertFalse(tr.isActive(0));
    }
}