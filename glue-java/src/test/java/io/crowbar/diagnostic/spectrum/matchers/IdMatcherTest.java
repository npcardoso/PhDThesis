package io.crowbar.diagnostic.spectrum.matchers;

import static org.junit.Assert.*;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;

import java.util.BitSet;
import java.util.Scanner;

import org.junit.Test;

public class IdMatcherTest {

	@Test
	public void testTransactions() {
		ActiveComponentMatcher a = new ActiveComponentMatcher();
		
		String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";

		Spectrum<Hit, ?> s = HitSpectrumUnserializer.unserialize(new Scanner(in));
		
		BitSet cs = a.matchComponents(s);
		BitSet ts = a.matchTransactions(s);
		
		IdMatcher id = new IdMatcher(ts, cs);
								
		assertEquals(id.matchTransactions(s).length(), 9);		
	}

	@Test
	public void testComponents() {
		ActiveComponentMatcher a = new ActiveComponentMatcher();
		
		String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";

		Spectrum<Hit, ?> s = HitSpectrumUnserializer.unserialize(new Scanner(in));
		
		BitSet cs = a.matchComponents(s);
		BitSet ts = a.matchTransactions(s);
		
		IdMatcher id = new IdMatcher(ts, cs);
								
		assertEquals(id.matchComponents(s).length(), 10);		
	}
}
