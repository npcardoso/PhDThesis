package io.crowbar.diagnostic.algorithms;

import static org.junit.Assert.*;

import org.junit.Test;

public class AlgorithmFactoryTest {

	@Test
	public void testSetConfigs() {
		AlgorithmFactory af = new AlgorithmFactory();
		
		String name = "name";
		String value = "value";
		
		af.setConfig(name, value);
		
		assertTrue(af.getConfig(name).equals(value));
	}

	@Test
	public void testSet2Configs() {
		AlgorithmFactory af = new AlgorithmFactory();
		
		String name = "name";
		String value = "value";

		String name1 = "name1";
		String value1 = "value1";
		
		af.setConfig(name, value);
		
		assertTrue(af.getConfig(name).equals(value));
	}	

	@Test
	public void testGetConfigs() {
		AlgorithmFactory af = new AlgorithmFactory();
		
		assertEquals(af.getConfig("name"), null);
	}	

	
	@Test
	public void testRemoveConfig() {
		AlgorithmFactory af = new AlgorithmFactory();
		
		String name = "name";
		String value = "value";
		
		af.setConfig(name, value);
		
		af.removeConfig(name);
		
		assertEquals(af.getConfig("name"), null);
	}	
	
	@Test
	public void testRemoveEmpty() {
		AlgorithmFactory af = new AlgorithmFactory();
		
		af.removeConfig("name");
		
		assertEquals(af.getConfig("name"), null);
	}	

	@Test
	public void testCreateAlgorithm() {
		AlgorithmFactory af = new AlgorithmFactory();
		
		String name = "name";
		String value = "value";
		
		af.setConfig(name, value);
		
		Algorithm a = af.create("Ochiai");
		
		assertTrue(a.getName().equals("single_fault"));
		assertTrue(a.getConfig("name").equals("value"));
	}		
}
