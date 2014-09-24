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
import io.crowbar.rest.models.DiagnosticModel;
import io.crowbar.rest.models.DiagnosticSystemModel;
import io.crowbar.rest.models.IdListModel;
import io.crowbar.util.MergeStrategy;

import com.wordnik.swagger.annotations.*;
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
@Api(value = "/reports", description = "Operations about reports")
public final class DiagnosticReportHandler {
    private final Database db;
    private final JSONSerializer json;

    public DiagnosticReportHandler (Database db,
                                    JSONSerializer json) {
        this.db = db;
        this.json = json;
    }

    private DiagnosticEntry getDiagnosticEntry (int sessionId) {
        Map<Integer, DiagnosticEntry> entries = db.getDiagnostics();
        DiagnosticEntry e = entries.get(sessionId);

        if (e == null)
            throw new NotFoundException("Invalid session Id");

        return e;
    }

    private Diagnostic getDiagnostic (int sessionId,
                                      int conId) {
        DiagnosticEntry e = getDiagnosticEntry(sessionId);

        DiagnosticSystem ds = e.getDiagnosticSystem();
        DiagnosticReport dr = e.getDiagnosticReport();
        Connection c = ds.getConnections().get(conId);


        if (c == null)
            throw new NotFoundException("Invalid connection Id");

        return dr.getDiagnostic(c);
    }

    @GET
    @Produces("application/json")
    @ApiOperation(value = "/",
                  notes = "Retrieves the list of session ids.",
                  response = IdListModel.class)
    public String sessions () {
        return json.deepSerialize(new IdListModel(db.getDiagnostics().keySet()));
    }

    @GET
    @Path("/{sessionId}")
    @Produces("application/json")
    @ApiOperation(value = "/{sessionId}",
                  notes = "Retrieves the diagnostic engine used in particular session.",
                  response = DiagnosticSystemModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session Id.")})
    public String diagnosticSessions (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId) {
        DiagnosticEntry e = getDiagnosticEntry(sessionId);


        return json.deepSerialize(new DiagnosticSystemModel(e.getDiagnosticSystem()));
    }

    @GET
    @Produces("application/json")
    @Path("/{sessionId}/{conId}/raw")
    @ApiOperation(value = "/{sessionId}/{conId}/raw",
                  notes = "Retrieves the raw diagnostic for a particular connection in the diagnostic engine.",
                  response = DiagnosticModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session/view Ids.")})
    public String sendReportRaw (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                                 @ApiParam(value = "The connection's id") @PathParam("conId") int conId) {
        return json.deepSerialize(new DiagnosticModel(getDiagnostic(sessionId, conId)));
    }

    @GET
    @Produces("application/json")
    @Path("/{sessionId}/{conId}/visualization")
    @ApiOperation(value = "/{sessionId}/{conId}/visualization",
                  notes = "Retrieves the visualization input for a particular connection in the diagnostic engine.")
    public String sendReportVisualization (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                                           @ApiParam(value = "The connection's id") @PathParam("conId") int conId) {
        Diagnostic diag = getDiagnostic(sessionId, conId);

        SpectrumEntry e = db.getSpectra().get(sessionId);


        if (e == null)
            throw new NotFoundException("Invalid session Id");


        Spectrum s = e.getFinal();
        Spectrum original = e.getOriginal();
        TreeViewFactory tvf = new TreeViewFactory(s.getTree());
        tvf.addStage(new TestNodesMatcher(original));


        TreeView t = tvf.getView();
        List<Double> scores = t.updateScores(s.getScorePerNode(diag, MergeStrategy.SUM));

        return io.crowbar.messages.Messages.serialize(
            VisualizationMessages.issueRequest(t, scores));
    }
}