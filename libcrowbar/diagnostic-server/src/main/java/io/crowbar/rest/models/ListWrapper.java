package io.crowbar.rest.models;

import java.util.AbstractList;
import java.util.List;


public class ListWrapper<O, W>
extends AbstractList<W> {
    public interface WrapperFactory<O, W> {
        W create (O original);
    }


    private final List<O> original;
    private final WrapperFactory<O, W> factory;

    public ListWrapper (List<O> original,
                        WrapperFactory<O, W> factory) {
        this.original = original;
        this.factory = factory;
    }

    @Override
    public int size () {
        return original.size();
    }

    @Override
    public W get (int i) {
        O o = original.get(i);


        if (o == null)
            return null;

        return factory.create(o);
    }
}