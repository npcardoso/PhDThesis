package io.crowbar.diagnostic;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
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
    public void testEmptyConstructor () {
        try {
            Constructor<Candidate> constructor;
            constructor = Candidate.class.getDeclaredConstructor();

            constructor.setAccessible(true);

            Candidate c = constructor.newInstance();

            assertTrue(c.size() == 0);
        }
        catch (Exception e) {
            e.printStackTrace();
            fail("InstantiationException: " + e.getStackTrace().toString());
        }
    }

    /*
     * Add more data points to test with other cases
     */
    @DataPoint
    public static int lElems1 = 1;
    @DataPoint
    public static int lElems2 = 50;
    @DataPoint
    public static int lElems3 = 100;

    private int[] createArray (int len) {
        Random r = new Random();


        int[] arr = new int[len];

        for (int i = 0; i < arr.length; i++) {
            arr[i] = r.nextInt();
        }

        return arr;
    }

    private List<Integer> createList (int len) {
        Random r = new Random();


        List<Integer> lst = new ArrayList<Integer> ();

        for (int i = 0; i < len; i++) {
            Integer x = new Integer(r.nextInt());
            lst.add(x);
        }

        return lst;
    }

    @Theory()
    public void testArrayIntConstructor (int len) {
        int[] arr = createArray(len);

        Candidate c = new Candidate(arr);

        assertEquals(c.size(), arr.length);
    }

    @Theory()
    public void testListConstructor (int len) {
        List<Integer> lst = createList(len);

        Candidate c = new Candidate(lst);

        assertEquals(c.size(), len);
    }

    @Theory()
    public void testContainsWithArr (int len) {
        boolean chk = true;


        int[] arr = createArray(len);

        Candidate c = new Candidate(arr);

        for (int i = 0; i < arr.length; i++) {
            chk &= c.contains(arr[i]);
        }

        assertTrue(chk);
    }

    @Theory()
    public void testContainsWithArrIterator (int len) {
        boolean chk = true;


        int[] arr = createArray(len);

        Candidate c = new Candidate(arr);

        List<Integer> intList = new ArrayList<Integer> ();

        for (int index = 0; index < len; index++) {
            intList.add(arr[index]);
        }

        Iterator<Integer> it = c.iterator();

        while (it.hasNext()) {
            chk &= intList.contains(it.next());
        }

        assertTrue(chk);
    }

    @Theory()
    public void testContainsWithList (int len) {
        boolean chk = true;


        List<Integer> lst = createList(len);

        Candidate c = new Candidate(lst);

        for (int i = 0; i < lst.size(); i++) {
            chk &= c.contains(lst.get(i));
        }

        assertTrue(chk);
    }

    @Test
    public void testToString () {
        int[] arr = {
            1, 3, 1, 0, 1, 0
        };

        Candidate c = new Candidate(arr);

        assertTrue(c.toString().equals("{class='Candidate', probes=[0, 1, 3]}"));
    }
}