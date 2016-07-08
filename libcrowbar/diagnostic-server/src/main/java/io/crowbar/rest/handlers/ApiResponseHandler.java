package io.crowbar.rest.handlers;

import io.crowbar.rest.models.ApiResponseModel;

import java.io.OutputStream;
import java.io.IOException;
import java.lang.annotation.Annotation;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.ProcessingException;
import javax.ws.rs.Produces;
import javax.ws.rs.ext.MessageBodyWriter;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.MultivaluedMap;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import flexjson.JSONSerializer;
import java.lang.reflect.Type;
import java.io.OutputStreamWriter;

@Produces("application/json")
public class ApiResponseHandler implements MessageBodyWriter<ApiResponseModel> {
    private final JSONSerializer json;

    public ApiResponseHandler (JSONSerializer json) {
        this.json = json;
    }

    @Override
    public boolean isWriteable (Class< ? > type,
                                Type genericType,
                                Annotation[] annotations,
                                MediaType mediaType) {
        return type == ApiResponseModel.class;
    }

    @Override
    public long getSize (ApiResponseModel response,
                         Class< ? > type,
                         Type genericType,
                         Annotation[] annotations,
                         MediaType mediaType) {
        // deprecated by JAX-RS 2.0 and ignored by Jersey runtime
        return 0;
    }

    @Override
    public void writeTo (ApiResponseModel response,
                         Class< ? > type,
                         Type genericType,
                         Annotation[] annotations,
                         MediaType mediaType,
                         MultivaluedMap<String, Object> httpHeaders,
                         OutputStream entityStream)
    throws IOException, WebApplicationException {
        try {
            OutputStreamWriter w = new OutputStreamWriter(entityStream);
            json.deepSerialize(response, w);
            w.flush();
            w.close();
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }
}