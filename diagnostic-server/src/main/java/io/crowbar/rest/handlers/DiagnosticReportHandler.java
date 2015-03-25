package io.crowbar.rest.handlers;

import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.diagnostic.Connection;
import io.crowbar.diagnostic.DiagnosticReport;
import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.messages.VisualizationMessages;
import io.crowbar.rest.database.Database;
import io.crowbar.rest.models.ApiResponseModel;
import io.crowbar.rest.models.DiagnosticModel;
import io.crowbar.rest.models.DiagnosticReportModel;
import io.crowbar.rest.models.DiagnosticSystemModel;
import io.crowbar.rest.models.IndexListModel;
import io.crowbar.rest.models.SpectrumModel;
import io.crowbar.util.MergeStrategy;

import com.wordnik.swagger.annotations.*;
import java.util.List;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.PathParam;


@Path("/reports")
@Produces("application/json")
@Api(value = "/reports", description = "Operations about reports")
public final class DiagnosticReportHandler {
    private final Database db;

    public DiagnosticReportHandler (Database db) {
        this.db = db;
    }

    private Diagnostic getDiagnostic (int drId,
                                      int conId) {
        DiagnosticReportModel drm = db.getDiagnosticReports().get(drId);


        if (drm == null)
            throw new NotFoundException("Invalid diagnostic report Id");

        DiagnosticSystem ds = db.getDiagnosticSystems().get(drm.getDiagnosticSystemId());
        DiagnosticReport dr = drm.getOriginal();
        Connection c = ds.getConnections().get(conId);

        if (c == null)
            throw new NotFoundException("Invalid connection Id");

        return dr.getDiagnostic(c);
    }

    @GET
    @ApiOperation(value = "/",
                  notes = "Retrieves the list of diagnostic report ids.",
                  response = IndexListModel.class)
    public ApiResponseModel<IndexListModel> getDiagnosticReportIds () {
        IndexListModel il = new IndexListModel(db.getDiagnosticReports().elements().size());


        return new ApiResponseModel<IndexListModel> (il);
    }

    @GET
    @Path("/{drId}/{conId}/raw")
    @ApiOperation(value = "/{drId}/{conId}/raw",
                  notes = "Retrieves the raw diagnostic for a particular connection in a diagnostic report.",
                  response = DiagnosticModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid diagnostic report/connection Id.")})
    public ApiResponseModel<DiagnosticModel> getDiagnosticReportRaw (@ApiParam(value = "The diagnostic reports's id") @PathParam("drId") int drId,
                                                                     @ApiParam(value = "The connection's id") @PathParam("conId") int conId) {
        DiagnosticModel d = new DiagnosticModel(getDiagnostic(drId, conId));


        return new ApiResponseModel<DiagnosticModel> (d);
    }

    @GET
    @Path("/{drId}/{conId}/visualization")
    @ApiOperation(value = "/{drId}/{conId}/visualization",
                  notes = "Retrieves the visualization input for a particular connection in the diagnostic engine.")
    public ApiResponseModel getDiagnosticReportVisualization (@ApiParam(value = "The diagnostic reports's id") @PathParam("drId") int drId,
                                                              @ApiParam(value = "The connection's id") @PathParam("conId") int conId) {
        Diagnostic diag = getDiagnostic(drId, conId);
        DiagnosticReportModel drm = db.getDiagnosticReports().get(drId);

        SpectrumModel se = db.getSpectra().get(drm.getSpectrumId());


        Spectrum s = se.getOriginal();


        List<Double> scores = s.getScorePerNode(diag, MergeStrategy.SUM);
        /*
         * //TODO: Refactor
         * Spectrum original = e.getOriginal();
         * TreeViewFactory tvf = new TreeViewFactory(s.getTree());
         * tvf.addStage(new TestNodesMatcher(original));
         * TreeView t = tvf.getView();
         * scores = t.updateScores(scores);
         */

        return new ApiResponseModel(VisualizationMessages.issueRequest(s.getTree(), scores, s.getFreqsPerNode()));
    }
}