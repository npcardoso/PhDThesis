package io.crowbar.diagnostic.handlers;

import java.util.List;
import java.util.LinkedList;

import javax.ws.rs.Produces;
import javax.ws.rs.GET;
import javax.ws.rs.Path;

@Path("/")
public class StaticLinksHandler {
    private List<String> links = new LinkedList<String> ();

    public void addLink (String link) {
        links.add(link);
    }

    @GET
    @Produces("text/html")
    public final String handle () {
        StringBuilder str = new StringBuilder();


        for (String l : links) {
            str.append(l);
        }

        return str.toString();
    }
}