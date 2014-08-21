package io.crowbar.diagnostic;

import java.lang.reflect.Type;
import java.util.Set;
import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;

import flexjson.ObjectFactory;
import flexjson.ObjectBinder;
import flexjson.JsonNumber;


public final class Candidate
    implements Iterable<Integer> {
    public static final class JSONObjectFactory implements ObjectFactory {
        public Object instantiate(ObjectBinder context, Object value, Type targetType, Class targetClass) {
            Candidate c = new Candidate();
            if(value instanceof Collection) {
                for(Object o: (Collection) value) {
                    if(o instanceof JsonNumber)
                        c.elements.add(((JsonNumber)o).intValue());
                    else
                        throw context.cannotConvertValueToTargetType(o, JsonNumber.class);}
            }
            else {
                throw context.cannotConvertValueToTargetType(value, Collection.class);
            }
            return c;
        }
    }

    private final Set<Integer> elements;

    private Candidate () {
        this.elements = new HashSet();
    }

    Candidate (int[] elements) {
        this.elements = new HashSet();

        for (int e:elements) {
            this.elements.add(e);
        }
    }

    Candidate (List<Integer> elements) {
        this.elements = new HashSet(elements);
    }

    public int size() {
        return elements.size();
    }

    public Iterator<Integer> iterator() {
        return elements.iterator();
    }

    @Override
    public String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Candidate', ");
        str.append("components=[");
        boolean first = true;

        for (int c : this) {
            if (!first)
                str.append(", ");

            str.append(c);
            first = false;
        }

        str.append("]}");

        return str.toString();
    }
}
