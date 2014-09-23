package io.crowbar.diagnostic.handlers;

import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.Connection;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.database.Database;
import io.crowbar.diagnostic.database.DiagnosticEntry;
import io.crowbar.diagnostic.spectrum.Spectrum;

import flexjson.JSONSerializer;
import java.util.Map;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.PathParam;
import javax.ws.rs.core.UriBuilder;
import javax.ws.rs.core.UriInfo;
import javax.ws.rs.core.Context;


@Path("/reports")
public final class DiagnosticReportHandler {
    private final Database db;
    private final JSONSerializer json;

    public DiagnosticReportHandler (Database db,
                                    JSONSerializer json) {
        this.db = db;
        this.json = json;
    }

    @GET
    @Produces("text/html")
    public String links (@Context UriInfo uriInfo) {
        StringBuilder str = new StringBuilder();


        Map<String, ? > entries = db.getDiagnostics();

        for (String id : entries.keySet()) {
            UriBuilder uri = uriInfo.getAbsolutePathBuilder();
            str.append("<div><a href=\"" + uri.path(id).build() + "/\">" + id + "</a></div>\n");
        }

        return str.toString();
    }

    @GET
    @Produces("text/html")
    @Path("/{specId}")
    public String links (@PathParam("specId") String specId,
                         @Context UriInfo uriInfo) {
        StringBuilder str = new StringBuilder();


        Map<String, DiagnosticEntry> entries = db.getDiagnostics();
        DiagnosticEntry e = entries.get(specId);

        if (e == null)
            throw new NotFoundException();

        DiagnosticSystem ds = e.getDiagnosticSystem();

        for (Connection c : ds.getConnections()) {
            UriBuilder uri = uriInfo.getAbsolutePathBuilder();
            str.append("<a href=\"" + uri.path("" + c.getId()).build() + "/\">" + c + "</a>\n");
        }

        return str.toString();
    }

    @GET
    @Produces("application/json")
    @Path("/{specId}/{conId}")
    public String sendReport (@PathParam("specId") String specId,
                              @PathParam("conId") int conId) {
        Map<String, DiagnosticEntry> entries = db.getDiagnostics();
        DiagnosticEntry e = entries.get(specId);

        if (e == null)
            throw new NotFoundException();

        DiagnosticSystem ds = e.getDiagnosticSystem();
        DiagnosticReport dr = e.getDiagnosticReport();

        Connection c = ds.getConnections().get(conId);

        if (c == null)
            throw new NotFoundException();

        Diagnostic diag = dr.getDiagnostic(c);


        return json.deepSerialize(diag);
    }
}