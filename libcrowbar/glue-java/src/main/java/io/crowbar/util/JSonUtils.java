package io.crowbar.util;

import flexjson.JSONSerializer;
import flexjson.transformer.AbstractTransformer;

public final class JSonUtils {
    /**
     * @brief This class is used to filter null elements from json serialization
     */
    private static class ExcludeTransformer extends AbstractTransformer {
        @Override
        public Boolean isInline () {
            return true;
        }

        @Override
        public void transform (Object object) {}
    }


    public static JSONSerializer getSerializer () {
        return new JSONSerializer()
               .exclude("*.class")
               .transform(new ExcludeTransformer(), void.class); // Remove null(optional) stuff
    }

    public static JSONSerializer getPrettySerializer () {
        return getSerializer().prettyPrint(true);
    }
}