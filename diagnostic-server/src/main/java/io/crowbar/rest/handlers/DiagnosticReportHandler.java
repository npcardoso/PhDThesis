package io.crowbar.rest.handlers;

import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.Connection;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.TreeView;
import io.crowbar.diagnostic.spectrum.TreeViewFactory;
import io.crowbar.diagnostic.spectrum.matchers.tree.TestNodesMatcher;
import io.crowbar.messages.VisualizationMessages;
import io.crowbar.rest.database.Database;
import io.crowbar.rest.database.DiagnosticEntry;
import io.crowbar.rest.database.SpectrumEntry;
import io.crowbar.util.MergeStrategy;

import flexjson.JSONSerializer;
import java.util.List;
import java.util.Map;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.PathParam;
import javax.ws.rs.core.UriBuilder;
import javax.ws.rs.core.UriInfo;
import javax.ws.rs.core.Context;
import javax.ws.rs.QueryParam;


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
            str.append("<div><a href=\"" + uri.path(id).build() + "\">" + id + "</a></div>\n");
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
            str.append("<div><a href=\"" + uri.path("" + c.getId()).build() + "\">" + c + "</a></div>\n");
            uri.queryParam("type", "visualization");

            UriBuilder uri2 = uriInfo.getAbsolutePathBuilder();
            String vizURI = uri2.path("../../visualizations/index.html").build() + "#jsonLoad_" + uri.build();
            str.append("<div><a href=\"" + vizURI + "\">Visualization (" + c + ")</a></div>\n");
        }

        return str.toString();
    }

    @GET
    @Produces("application/json")
    @Path("/{specId}/{conId}")
    public String sendReport (@PathParam("specId") String specId,
                              @PathParam("conId") int conId,
                              @QueryParam("type") String returnType) {
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

        if (returnType == null)
            return sendReportNormal(diag);

        if (returnType.equals("visualization"))
            return sendReportVisualization(specId, diag);

        throw new NotFoundException();
    }

    public String sendReportVisualization (String specId,
                                           Diagnostic diag) {
        SpectrumEntry e = db.getSpectra().get(specId);


        if (e == null)
            throw new NotFoundException();


        Spectrum s = e.getFinal();
        Spectrum original = e.getOriginal();
        TreeViewFactory tvf = new TreeViewFactory(s.getTree());
        tvf.addStage(new TestNodesMatcher(original));


        TreeView t = tvf.getView();
        List<Double> scores = t.updateScores(s.getScorePerNode(diag, MergeStrategy.SUM));

        String jsonRequest = io.crowbar.messages.Messages.serialize(
            VisualizationMessages.issueRequest(t, scores));

        return jsonRequest;
    }

    public String sendReportNormal (Diagnostic diag) {
        return json.deepSerialize(diag);
    }
}