package io.crowbar.instrumentation.runtime;


import flexjson.JSONSerializer;
import flexjson.transformer.AbstractTransformer;

import java.util.List;

public final class TreeJSONSerializer {
    public static final class TreeNodeTransformer extends AbstractTransformer {
        @Override
        public void transform (Object object) {
            Tree t = (Tree) object;


            getContext().writeOpenArray();
            boolean first = true;

            for (Node n : t) {
                getContext().writeOpenObject();
                getContext().writeName("id");
                getContext().transform(n.getId());
                getContext().writeComma();
                getContext().writeName("name");
                getContext().transform(n.getName());
                getContext().writeComma();
                getContext().writeName("parent_id");
                getContext().transform(n.getParentId());
                getContext().writeComma();

                List<Integer> children = n.getChildren();

                if (children.size() > 0) {
                    getContext().writeComma();
                    getContext().writeName("children");
                    getContext().transform(children);
                }

                getContext().writeCloseObject();

                if (!first)
                    getContext().writeComma();

                first = false;
            }

            getContext().writeCloseArray();
        }
    }

    public static String serialize (Tree t) {
        return serialize(t, new JSONSerializer());
    }

    public static String serialize (Tree t,
                                    JSONSerializer serializer) {
        return serializer
               .transform(new TreeNodeTransformer(), Tree.class)
               .prettyPrint(true)
               .serialize(t);
    }
}