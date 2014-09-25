package io.crowbar.rest.handlers;


import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.rest.database.Database;
import io.crowbar.rest.database.SpectrumEntry;
import io.crowbar.rest.models.ApiResponseModel;
import io.crowbar.rest.models.IdListModel;
import io.crowbar.rest.models.SpectrumModel;
import io.crowbar.rest.models.TreeModel;

import com.wordnik.swagger.annotations.*;
import java.util.Map;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.PathParam;
import java.util.List;
import io.crowbar.rest.models.TransactionModel;
import io.crowbar.rest.models.ProbeModel;


@Path("/spectra")
@Produces("application/json")
@Api(value = "/spectra", description = "Operations about spectra")
public final class SpectraHandler {
    private final Database db;

    public SpectraHandler (Database db) {
        this.db = db;
    }

    private SpectrumModel getSpectrum (int sessionId,
                                       int viewId) {
        Map<Integer, SpectrumEntry> entries = db.getSpectra();
        SpectrumEntry e = entries.get(sessionId);

        if (e == null)
            throw new NotFoundException("Invalid session Id");


        Spectrum s = null;

        if (viewId == 0)
            s = e.getOriginal();
        else if (viewId > 0)
            s = e.getViews().get(viewId - 1);

        if (s == null)
            throw new NotFoundException("Invalid View Id");

        return new SpectrumModel(s);
    }

    @GET
    @ApiOperation(value = "/",
                  notes = "Retrieves the list of session ids.",
                  response = Integer.class)
    public ApiResponseModel<Iterable<Integer> > sessions () {
        return new ApiResponseModel<Iterable<Integer> > (db.getSpectra().keySet());
    }

    @GET
    @Path("/{sessionId}")
    @ApiOperation(value = "/{sessionId}",
                  notes = "Retrieves the list of views for a particular session.")
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session Id.")})
    public String diagnosticSessions (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId) {
        return "[\"TODO\"]";
    }

    @GET
    @Path("/{sessionId}/{viewId}")
    @ApiOperation(value = "/{sessionId}/{viewId}",
                  notes = "Retrieves a spectrum view.",
                  response = SpectrumModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session/view Ids.")})
    public ApiResponseModel<SpectrumModel> sendSpectrum (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                                                         @ApiParam(value = "The view's id") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(sessionId, viewId);


        return new ApiResponseModel<SpectrumModel> (s);
    }

    @GET
    @Path("/{sessionId}/{viewId}/tree")
    @ApiOperation(value = "/{sessionId}/{viewId}/tree",
                  notes = "Retrieves the tree for a spectrum view.",
                  response = TreeModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session/view Ids.")})
    public ApiResponseModel<TreeModel> sendTree (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                                                 @ApiParam(value = "The view's id") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(sessionId, viewId);


        return new ApiResponseModel<TreeModel> (s.getTree());
    }

    @GET
    @Path("/{sessionId}/{viewId}/probes")
    @ApiOperation(value = "/{sessionId}/{viewId}/probes",
                  notes = "Retrieves the probes for a spectrum view.",
                  response = ProbeModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session/view Ids.")})
    public ApiResponseModel<List<ProbeModel> > sendProbes (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                                                           @ApiParam(value = "The view's id") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(sessionId, viewId);


        return new ApiResponseModel<List<ProbeModel> > (s.getProbes());
    }

    @GET
    @Path("/{sessionId}/{viewId}/transactions")
    @ApiOperation(value = "/{sessionId}/{viewId}/transactions",
                  notes = "Retrieves the transactions for a spectrum view.",
                  response = TransactionModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session/view Ids.")})
    public ApiResponseModel<List<TransactionModel> > sendTransactions (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                                                                       @ApiParam(value = "The view's id") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(sessionId, viewId);


        return new ApiResponseModel<List<TransactionModel> > (s.getTransactions());
    }
}