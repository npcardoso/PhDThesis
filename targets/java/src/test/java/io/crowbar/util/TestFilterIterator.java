/*
 *  public void filterIteratorNoFilter() {
 *      tFilterIterator it(100);
 *
 *
 *      tId id = 0;
 *
 *
 *      while (it.next())
 *          assertTrue(++id == it.get());
 *
 *      assertTrue(id == 100);
 *      assertTrue(it.get() == 0);
 *
 *      for (id = 0; id < 100; id++) {
 *          it.set(id);
 *
 *          while (it.next())
 *              assertTrue(++id == it.get());
 *
 *          assertTrue(id == 100);
 *          assertTrue(it.get() == 0);
 *      }
 *  }
 *
 *  public void filterIterator() {
 *      tFilter f1;
 *
 *
 *      randomize(f1, 25, 1, 99);
 *
 *      tFilterIterator it(100, &f1);
 *
 *
 *      tId id = 0;
 *
 *      while (it.next()) {
 *          assertTrue(!f1.isFiltered(it.get()));
 *
 *          while (++id != it.get())
 *              assertTrue(f1.isFiltered(id));
 *      }
 *
 *      assertTrue(id == 100);
 *      assertTrue(it.get() == 0);
 *
 *      for (id = 0; id < 100; id++) {
 *          it.set(id);
 *
 *          while (it.next()) {
 *              assertTrue(!f1.isFiltered(it.get()));
 *
 *              while (++id != it.get())
 *                  assertTrue(f1.isFiltered(id));
 *          }
 *
 *          assertTrue(id == 100);
 *          assertTrue(it.get() == 0);
 *      }
 *  }
 *
 *
 * }
 *
 */