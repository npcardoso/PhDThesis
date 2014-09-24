package io.crowbar.rest.handlers;

import java.io.IOException;
import java.io.File;
import java.io.FileInputStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.charset.Charset;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.NotFoundException;

@Path("/")
public final class SwaggerHandler {
    private String root;

    public String getRoot () {
        return root;
    }

    public void setRoot (String root) throws IOException {
        this.root = new File(root).getCanonicalFile().getPath();
    }

    public SwaggerHandler (String root) throws IOException {
        setRoot(root);
    }

    @GET
    @Produces("application/json")
    @Path("/{file:[a-zA-Z0-9_]*\\.json$}")
    public String sendSpectrum (@PathParam("file") String f) throws IOException {
        File file = new File(root + "/" + f).getCanonicalFile();


        System.out.println("trying to access: " + file);

        if (!file.getPath().startsWith(root)) {
            throw new NotFoundException("Invalid path");
        } else if (!file.isFile()) {
            throw new NotFoundException("File not found");
        }

        byte[] encoded = Files.readAllBytes(file.toPath());
        return new String(encoded, Charset.defaultCharset());
    }
}