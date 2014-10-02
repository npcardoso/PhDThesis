package io.crowbar.util;

import static org.junit.Assert.assertEquals;

import java.util.NoSuchElementException;
import java.util.Random;

import org.junit.Test;


public class FilterIteratorTest {
    void randomize (EditableFilter f,
                    int count,
                    int min,
                    int max) {
        Random rand = new Random();


        for (int i = 0; i < count; i++) {
            int id = rand.nextInt(max - min) + min;

            if (f.isFiltered(id))
                i--;
            else {
                f.filter(id);
                assertEquals(true, f.isFiltered(id));
            }
        }

        assertEquals(count, f.getFilteredCount());
    }

    @Test
    public void filterIterator () {
        EditableFilter filter = new FastFilter();


        randomize(filter, 1000, 1, 10001);

        FilterIterator fit = new FilterIterator(filter);

        while (fit.hasNext()) {
            Integer i = fit.next();
            // do something with i
        }

        assertEquals(false, fit.hasNext());
    }

    @Test(expected = UnsupportedOperationException.class)
    public void filterIteratorRemove () {
        EditableFilter filter = new FastFilter();


        randomize(filter, 1000, 1, 10001);

        FilterIterator fit = new FilterIterator(filter);

        fit.remove();
    }

    @Test(expected = NoSuchElementException.class)
    public void filterIteratorNextProblem () {
        EditableFilter filter = new FastFilter();


        randomize(filter, 1000, 1, 10001);

        FilterIterator fit = new FilterIterator(filter);

        while (fit.hasNext()) {
            Integer i = fit.next();
            // do something with i
        }

        fit.next();
    }
}