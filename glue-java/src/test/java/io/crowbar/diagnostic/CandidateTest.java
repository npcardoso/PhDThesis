package io.crowbar.diagnostic;

import static org.junit.Assert.*;

import java.lang.reflect.Constructor;
import java.util.Random;

import org.junit.Test;

public class CandidateTest {	
	@Test
	public void testEmptyConstructor() {
		try {
	        Constructor<Candidate> constructor;
	        constructor = Candidate.class.getDeclaredConstructor();
	        
	        constructor.setAccessible(true);
	        
	        Candidate c = constructor.newInstance();
	        	        
	        assertTrue(c.size() == 0);
	        assertTrue(c.toString().equals("{class='Candidate', components=[]}"));
			
		} catch (Exception e) {
			e.printStackTrace();
			fail("InstantiationException: " + e.getStackTrace().toString());
		} 
	}
	
	@Test
	public void testArrayIntConstructor() {
		Random r = new Random();
		int maxElems = r.nextInt(100);
		
		int[] arr = new int[maxElems];
		
		for(int i = 0; i < arr.length; i++) {
		    arr[i] = r.nextInt();
		}
		
		Candidate c = new Candidate(arr);
		
		assert(c.size() == arr.length);

	}	

}
