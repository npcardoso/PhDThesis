package io.crowbar.rest.handlers;

import io.crowbar.rest.models.ApiResponseModel;

import javax.ws.rs.ClientErrorException;
import javax.ws.rs.core.Response;
import javax.ws.rs.ext.Provider;
import javax.ws.rs.ext.ExceptionMapper;
import flexjson.JSONSerializer;

@Provider
public class ExceptionHandler implements ExceptionMapper<ClientErrorException> {
    private final JSONSerializer json;

    public ExceptionHandler (JSONSerializer json) {
        this.json = json;
    }

    public Response toResponse (ClientErrorException ex) {
        Response r = ex.getResponse();


        Response.ResponseBuilder rb = Response.fromResponse(r);
        return rb.entity(json.deepSerialize(new ApiResponseModel(r.getStatus(), ex.getMessage()))).
               type("application/json").
               build();
    }
}