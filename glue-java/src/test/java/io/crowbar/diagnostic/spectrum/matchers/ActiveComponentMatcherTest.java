package io.crowbar.diagnostic.spectrum.matchers;

import static org.junit.Assert.assertEquals;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;

import java.util.BitSet;
import java.util.Scanner;

import org.junit.Test;

//TODO: Needs to be improved
public class ActiveComponentMatcherTest {

	@Test
	public void testTransactions() {
		ActiveComponentMatcher a = new ActiveComponentMatcher();
		
		String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";

		Spectrum<Hit, ?> s = HitSpectrumUnserializer.unserialize(new Scanner(in));
		
		BitSet bs = a.matchComponents(s);
						
		assertEquals(bs.length(), 10);		
	}

	@Test
	public void testTransactionsDefaultTrue() {
		ActiveComponentMatcher a = new ActiveComponentMatcher(true);
		
		String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";

		Spectrum<Hit, ?> s = HitSpectrumUnserializer.unserialize(new Scanner(in));
		
		BitSet bs = a.matchComponents(s);
						
		assertEquals(bs.length(), 10);		
	}	

	@Test
	public void testTransactionsDefaultFalse() {
		ActiveComponentMatcher a = new ActiveComponentMatcher(false);
		
		String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";

		Spectrum<Hit, ?> s = HitSpectrumUnserializer.unserialize(new Scanner(in));
		
		BitSet bs = a.matchComponents(s);
						
		assertEquals(bs.length(), 10);		
	}		
}