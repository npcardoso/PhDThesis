package io.crowbar.diagnosis.runners.messages;

import flexjson.JSON;
import flexjson.JSONSerializer;
import flexjson.JSONDeserializer;
import flexjson.locators.TypeLocator;
import flexjson.transformer.AbstractTransformer;

public final class Messages {
    private static class ExcludeTransformer extends AbstractTransformer {
        @Override
        public Boolean isInline () {return true;}

        @Override
        public void transform (Object object) {}
    }


    private static TypeLocator<String> responseBinder = new TypeLocator<String> ("type");
    private static TypeLocator<String> requestBinder = new TypeLocator<String> ("type");

    static void registerResponse (String name,
                                  Class response) {
        responseBinder = responseBinder.add(name, response);
    }

    static void registerRequest (String name,
                                 Class response) {
        requestBinder = requestBinder.add(name, response);
    }

    public static String serialize (Message m) {
        String json = new JSONSerializer()
                      .transform(new ExcludeTransformer(), void.class) // Remove null(optional) stuff
                      .exclude("*.class")
                      // .prettyPrint(true)
                      .deepSerialize(m);


        return json;
    }

    public static JSONDeserializer<Response> getResponseDeserializer () {
        return new JSONDeserializer<Response> ().use(null, responseBinder);
    }

    public static JSONDeserializer<Request> getRequestDeserializer () {
        return new JSONDeserializer<Request> ().use(null, requestBinder);
    }
}