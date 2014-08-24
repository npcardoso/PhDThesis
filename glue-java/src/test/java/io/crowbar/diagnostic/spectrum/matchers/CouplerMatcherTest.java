package io.crowbar.diagnostic.spectrum.matchers;

import static org.junit.Assert.*;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.activity.Hit;
import io.crowbar.diagnostic.spectrum.matchers.CouplerMatcher.Coupler;
import io.crowbar.diagnostic.spectrum.unserializers.HitSpectrumUnserializer;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.BitSet;
import java.util.Scanner;

import org.junit.Test;

public class CouplerMatcherTest {

	@Test
	public void testTransactions() {
		ActiveComponentMatcher a = new ActiveComponentMatcher();
		
		ValidTransactionMatcher v = new ValidTransactionMatcher();
		NegateMatcher n = new NegateMatcher(v);
		
		String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";

		Spectrum<Hit, ?> s = HitSpectrumUnserializer.unserialize(new Scanner(in));
		
		BitSet cs = a.matchComponents(s);
		BitSet ts = a.matchTransactions(s);

        Field[] allFields = CouplerMatcher.class.getDeclaredFields();
        allFields[0].setAccessible(true);
        try {
			Coupler coupler = (Coupler) allFields[0].get(null);
						
			CouplerMatcher cmat = new CouplerMatcher(coupler, a, n);
						
			assertEquals(cmat.matchTransactions(s).length(), 0);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}		

	
	@Test
	public void testComponentsAnd() {
		ActiveComponentMatcher a = new ActiveComponentMatcher();
		
		ValidTransactionMatcher v = new ValidTransactionMatcher();
		NegateMatcher n = new NegateMatcher(v);
		
		String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";

		Spectrum<Hit, ?> s = HitSpectrumUnserializer.unserialize(new Scanner(in));
		
		BitSet cs = a.matchComponents(s);
		BitSet ts = a.matchTransactions(s);

        Field[] allFields = CouplerMatcher.class.getDeclaredFields();
        allFields[0].setAccessible(true);
        try {
			Coupler coupler = (Coupler) allFields[0].get(null);
						
			CouplerMatcher cmat = new CouplerMatcher(coupler, a, n);
						
			assertEquals(cmat.matchComponents(s).length(), 0);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}	
}
