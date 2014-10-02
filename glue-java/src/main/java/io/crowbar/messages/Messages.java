package io.crowbar.messages;

import io.crowbar.util.JSonUtils;

import flexjson.JSONSerializer;
import flexjson.JSONDeserializer;
import flexjson.locators.TypeLocator;

public final class Messages {
    private static TypeLocator<String> responseBinder = new TypeLocator<String> ("type");
    private static TypeLocator<String> requestBinder = new TypeLocator<String> ("type");

    static void registerResponse (String name,
                                  Class< ? > response) {
        responseBinder = responseBinder.add(name, response);
    }

    static void registerRequest (String name,
                                 Class< ? > response) {
        requestBinder = requestBinder.add(name, response);
    }

    public static String serialize (Message m) {
        String json = JSonUtils.getSerializer().deepSerialize(m);


        return json;
    }

    public static JSONDeserializer<Response> getResponseDeserializer () {
        return new JSONDeserializer<Response> ()
               .use(null, responseBinder);
    }

    public static JSONDeserializer<Request> getRequestDeserializer () {
        return new JSONDeserializer<Request> ()
               .use(null, requestBinder);
    }
}