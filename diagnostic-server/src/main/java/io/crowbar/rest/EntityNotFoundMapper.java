package io.crowbar.rest;

import javax.ws.rs.ext.Provider;

import javax.ws.rs.NotFoundException;

import javax.ws.rs.ext.ExceptionMapper;
import javax.ws.rs.core.Response;

@Provider
public class EntityNotFoundMapper implements ExceptionMapper<NotFoundException> {
    public Response toResponse (NotFoundException ex) {
        return Response.status(404).
               entity(ex.getMessage()).
               type("text/plain").
               build();
    }
}