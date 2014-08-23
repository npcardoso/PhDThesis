package io.crowbar.diagnostic;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.List;
import java.lang.reflect.Constructor;
import java.util.Random;

import org.junit.Test;
import org.junit.experimental.theories.DataPoint;
import org.junit.experimental.theories.Theories;
import org.junit.experimental.theories.Theory;
import org.junit.runner.RunWith;

@RunWith(Theories.class)
public class CandidateTest {	
	
	@Test
	public void testEmptyConstructor() {
		try {
	        Constructor<Candidate> constructor;
	        constructor = Candidate.class.getDeclaredConstructor();
	        
	        constructor.setAccessible(true);
	        
	        Candidate c = constructor.newInstance();
	        	        
	        assertTrue(c.size() == 0);			
		} catch (Exception e) {
			e.printStackTrace();
			fail("InstantiationException: " + e.getStackTrace().toString());
		} 
	}

	
	@DataPoint
    public static int lElems1 = 1;
	@DataPoint
    public static int lElems2 = 50;
	@DataPoint
    public static int lElems3 = 100;	

	private int[] createArray(int len) {
		Random r = new Random();
		
		int[] arr = new int[len];
		
		for(int i = 0; i < arr.length; i++) {
		    arr[i] = r.nextInt();
		}
		
		return arr;
	}
	
	private List<Integer> createList(int len) {
		Random r = new Random();
		
		List<Integer> lst = new ArrayList<Integer>();
		
		for(int i = 0; i < len; i++) {
			Integer x = new Integer(r.nextInt());
		    lst.add(x);
		}
		
		return lst;
	}	
	
	@Theory()
	public void testArrayIntConstructor(int len) {
		int[] arr = createArray(len);
		
		Candidate c = new Candidate(arr);
		
		assertEquals(c.size(), arr.length);

	}	

	@Theory()
	public void testListConstructor(int len) {
		List<Integer> lst = createList(len);
		
		Candidate c = new Candidate(lst);
		
		assertEquals(c.size(), len);
	}	

	
	@Theory()
	public void testContainsWithArr(int len) {
		int[] arr = createArray(len);
		
		Candidate c = new Candidate(arr);
		
		for(int i = 0; i < arr.length; i++) {
		    if(!c.contains(arr[i]))
		    	fail("Element not in Candidate!");
		}		
	}		
	
	@Theory()
	public void testContainsWithList(int len) {
		List<Integer> lst = createList(len);
		
		Candidate c = new Candidate(lst);
		
		for(int i = 0; i < lst.size(); i++) {
		    if(!c.contains(lst.get(i)))
		    	fail("Element not in Candidate!");
		}
	}		
}
