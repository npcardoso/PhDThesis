package io.crowbar.util;

import io.crowbar.util.EditableFilter;
import io.crowbar.util.FastFilter;

import java.util.Random;
import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.*;
import org.junit.Test;


public class TestFilter {
    void randomize (EditableFilter f, int count, int min, int max) {
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
    public void emptyTest () {
        EditableFilter filter = new FastFilter();


        assertEquals(0, filter.getFilteredCount());

        for (int i = 1; i <= 1000; i++) {
            assertEquals(false, filter.isFiltered(i));
        }

        assertEquals(-1, filter.nextFiltered(0));

        for (int i = 1; i <= 1000; i++) {
            assertEquals(i + 1, filter.next(i));
        }
    }

    @Test
    public void copyConstructorTest () {
        EditableFilter f1 = new FastFilter();


        for (int i = 0; i < 1000; i++) {
            f1.filter(i);
            assertEquals(true, f1.isFiltered(i));
            assertEquals(i, f1.getLastFiltered());
            EditableFilter f2 = new FastFilter(f1);
            assertEquals(f1, f2);
        }
    }

    @Test
    public void filterId () {
        Random rand = new Random();
        EditableFilter f1 = new FastFilter();
        EditableFilter f2;


        for (int i = 0; i < 1000; i++) {
            int id;

            do {
                id = rand.nextInt(10000);
            } while (f1.isFiltered(id));

            int maxSize = Math.max(f1.getLastFiltered(), id);


            assertEquals(i, f1.getFilteredCount());

            f2 = new FastFilter(f1);
            assertEquals(f1, f2);
            assertEquals(i, f1.getFilteredCount());
            assertEquals(i, f2.getFilteredCount());


            assertEquals(false, f1.isFiltered(id));
            f1.filter(id);
            assertEquals(maxSize, f1.getLastFiltered());
            assertEquals(true, f1.isFiltered(id));
            assertEquals(false, f2.isFiltered(id));
            assertEquals(i + 1, f1.getFilteredCount());

            f1.filter(id); // Filtering twice on purpose
            assertEquals(maxSize, f1.getLastFiltered());
            assertEquals(true, f1.isFiltered(id));
            assertEquals(false, f2.isFiltered(id));
            assertEquals(i + 1, f1.getFilteredCount());

            for (int j = 1; j <= maxSize; j++) {
                if (j == id)
                    assertEquals(true, f1.isFiltered(id));
                else
                    assertEquals(f1.isFiltered(j), f2.isFiltered(j));
            }
        }
    }

    @Test
    public void next () {
        EditableFilter f1 = new FastFilter();


        randomize(f1, 1000, 1, 10001);


        assertEquals(1000, f1.getFilteredCount());

        int next = 0;
        int unfilteredCount = 0;

        while ((next = f1.next(next)) < f1.getLastFiltered()) {
            unfilteredCount++;
        }

        assertEquals(unfilteredCount + f1.getFilteredCount(), f1.getLastFiltered());
    }

    @Test
    public void nextFiltered () {
        EditableFilter f1 = new FastFilter();


        randomize(f1, 1000, 1, 10001);


        assertEquals(1000, f1.getFilteredCount());

        int next = 0;
        int filteredCount = 0;

        while ((next = f1.nextFiltered(next)) >= 0) {
            filteredCount++;
        }

        assertEquals(filteredCount, f1.getFilteredCount());
    }

    @Test
    public void filterAll () {
        Random rand = new Random();
        EditableFilter f1 = new FastFilter();


        List<Integer> d = new ArrayList<Integer> ();

        f1.filterAll(d);

        for (int i = 1; i <= f1.getLastFiltered(); i++)
            assertFalse(f1.isFiltered(i));

        for (int i = 0; i < 200; i++) {
            d.add(rand.nextInt(10000));

            if (d.size() != i + 1)
                i--;
        }

        assertEquals(200, d.size());

        f1.filterAll(d);

        for (int i = 1; i <= f1.getLastFiltered(); i++)
            assertEquals(d.contains(i), f1.isFiltered(i));
    }
}