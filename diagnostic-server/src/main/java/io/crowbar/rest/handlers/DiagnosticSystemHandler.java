package io.crowbar.rest.handlers;

import io.crowbar.diagnostic.DiagnosticSystem;
import io.crowbar.rest.database.Database;
import io.crowbar.rest.models.ApiResponseModel;
import io.crowbar.rest.models.DiagnosticSystemModel;
import io.crowbar.rest.models.IndexListModel;

import java.util.ArrayList;
import java.util.List;

import com.wordnik.swagger.annotations.*;
import javax.ws.rs.*;


@Path("/diagnosticSystems")
@Produces("application/json")
@Api(value = "/diagnosticSystems", description = "Operations on diagnostic systems")
public final class DiagnosticSystemHandler {
    private final Database db;

    public DiagnosticSystemHandler (Database db) {
        this.db = db;
    }

    private DiagnosticSystem getDiagnosticSystemPvt (int diagSystemId) {
        DiagnosticSystem e = db.getDiagnosticSystems().get(diagSystemId);


        if (e == null)
            throw new NotFoundException("Invalid diagnostic system Id.");

        return e;
    }

    @GET
    @ApiOperation(value = "/",
                  notes = "Retrieves the list of diagnostic system ids.",
                  response = IndexListModel.class)
    public ApiResponseModel<IndexListModel> getDiagnosticSystemIds () {
        IndexListModel il = new IndexListModel(db.getDiagnosticSystems().elements().size());


        return new ApiResponseModel<IndexListModel> (il);
    }

    @GET
    @Path("/{diagSystemId}")
    @ApiOperation(value = "/{diagSystemId}",
                  notes = "Retrieves the diagnostic engine used in particular session.",
                  response = DiagnosticSystemModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid diagnostic system Id.")})
    public ApiResponseModel<DiagnosticSystemModel> getDiagnosticSystem (@ApiParam(value = "The diagnostic system's id") @PathParam("diagSystemId") int diagSystemId) {
        DiagnosticSystemModel ds = new DiagnosticSystemModel(getDiagnosticSystemPvt(diagSystemId));


        return new ApiResponseModel<DiagnosticSystemModel> (ds);
    }
}