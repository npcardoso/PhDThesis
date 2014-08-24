package io.crowbar.diagnostic.spectrum.unserializers;

import static org.junit.Assert.*;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.serializers.HitSpectrumSerializer;

import java.util.Scanner;

import org.junit.Test;
import org.junit.experimental.theories.Theory;

public class HitSpectrumUnserializerTest {

	@Test
	public void testSerializer() {
		String in = "10 9 0 1 0 0 0 0 0 0 0 0 0.0 0 0 0 1 0 0 0 0 0 0 0.0 1 0 0 0 0 1 0 0 0 0 0.0 0 0 0 0 0 0 0 1 0 0 0.0 0 0 0 0 0 0 0 0 1 0 0.0 0 0 1 0 0 0 0 0 0 0 1.0 0 0 0 0 1 0 0 0 0 0 1.0 0 0 0 0 0 0 1 0 0 0 1.0 0 0 0 0 0 0 0 0 0 1 1.0";

		Spectrum s = HitSpectrumUnserializer.unserialize(new Scanner(in));
		
		s.toString();
		
	    //TODO Assert
	}

}
