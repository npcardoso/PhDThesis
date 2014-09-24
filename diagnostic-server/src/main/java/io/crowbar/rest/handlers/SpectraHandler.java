package io.crowbar.rest.handlers;


import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.rest.database.Database;
import io.crowbar.rest.database.SpectrumEntry;
import io.crowbar.rest.models.SpectrumModel;

import com.wordnik.swagger.annotations.*;
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


@Path("/spectra")
@Api(value = "/spectra", description = "Operations about spectra")
public final class SpectraHandler {
    private final Database db;
    private final JSONSerializer json;

    public SpectraHandler (Database db,
                           JSONSerializer json) {
        this.db = db;
        this.json = json;
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
    @Produces("application/json")
    @ApiOperation(value = "/",
                  notes = "Retrieves the list of session ids.")
    public String sessions () {
        return json.deepSerialize(db.getSpectra().keySet());
    }

    @GET
    @Path("/{sessionId}")
    @Produces("application/json")
    @ApiOperation(value = "/{sessionId}",
                  notes = "Retrieves the list of views for a particular session.",
                  response = Iterable.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session Id.")})
    public String diagnosticSessions (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId) {
        return "{'TODO':'TODO'}";
    }

    @GET
    @Path("/{sessionId}/{viewId}")
    @Produces("application/json")
    @ApiOperation(value = "/{sessionId}/{viewId}",
                  notes = "Retrieves a spectrum view.",
                  response = SpectrumModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session/view Ids.")})
    public String sendSpectrum (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                                @ApiParam(value = "The view's id") @PathParam("viewId") int viewId) {
        return json.deepSerialize(getSpectrum(sessionId, viewId));
    }

    @GET
    @Path("/{sessionId}/{viewId}/tree")
    @Produces("application/json")
    @ApiOperation(value = "/{sessionId}/{viewId}/tree",
                  notes = "Retrieves the tree for a spectrum view.", response = SpectrumModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session/view Ids.")})
    public String sendTree (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                            @ApiParam(value = "The view's id") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(sessionId, viewId);


        return json.deepSerialize(s.getTree());
    }

    @GET
    @Path("/{sessionId}/{viewId}/probes")
    @Produces("application/json")
    @ApiOperation(value = "/{sessionId}/{viewId}/probes",
                  notes = "Retrieves the probes for a spectrum view.", response = Iterable.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session/view Ids.")})
    public String sendProbes (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                              @ApiParam(value = "The view's id") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(sessionId, viewId);


        return json.deepSerialize(s.getProbes());
    }

    @GET
    @Path("/{sessionId}/{viewId}/transactions")
    @Produces("application/json")
    @ApiOperation(value = "/{sessionId}/{viewId}/transactions", notes = "Retrieves the transactions for a spectrum view.", response = SpectrumModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session/view Ids.")})
    public String sendTransactions (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId,
                                    @ApiParam(value = "The view's id") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(sessionId, viewId);


        return json.deepSerialize(s.getTransactions());
    }
}