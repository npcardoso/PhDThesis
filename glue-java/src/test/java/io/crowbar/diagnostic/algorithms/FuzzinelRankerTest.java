package io.crowbar.diagnostic.algorithms;

import static org.junit.Assert.assertTrue;

import org.junit.experimental.theories.DataPoints;
import org.junit.experimental.theories.Theories;
import org.junit.experimental.theories.Theory;
import org.junit.runner.RunWith;

@RunWith(Theories.class)
public class FuzzinelRankerTest {

	@DataPoints
	public static boolean[] booleanVals = {true, false};
	
	@Theory()
	public void testConfidence(boolean c, boolean f) {
		FuzzinelRanker fr = new FuzzinelRanker();
		fr.useConfidence(c);
		fr.useFuzzyError(f);
		
		Algorithm a = fr.getAlgorithm();
		
		assertTrue(a.getConfig("confidence").equals("" + c));
	}

	@Theory()
	public void testFuzzy_error(boolean c, boolean f) {
		FuzzinelRanker fr = new FuzzinelRanker();
		fr.useConfidence(c);
		fr.useFuzzyError(f);
		
		Algorithm a = fr.getAlgorithm();
		
		assertTrue(a.getConfig("fuzzy_error").equals("" + f));
	}	
}
