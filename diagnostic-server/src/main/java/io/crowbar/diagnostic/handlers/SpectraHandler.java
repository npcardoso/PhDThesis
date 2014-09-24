package io.crowbar.diagnostic.handlers;


import io.crowbar.diagnostic.database.Database;
import io.crowbar.diagnostic.database.SpectrumEntry;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.handlers.spectrum.SpectrumModel;

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
import com.wordnik.swagger.annotations.*;


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

    @GET
    @Produces("text/html")
    public String links (@Context UriInfo uriInfo) {
        StringBuilder str = new StringBuilder();


        Map<String, ? > entries = db.getSpectra();

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


        Map<String, SpectrumEntry> entries = db.getSpectra();
        SpectrumEntry e = entries.get(specId);

        if (e == null)
            throw new NotFoundException();

        UriBuilder uri = uriInfo.getAbsolutePathBuilder();

        str.append("<div><a href=\"" + uri.path("0").build() + "/\">original</a></div>\n");

        for (int i = 0; i < e.getViews().size(); i++) {
            uri = uriInfo.getAbsolutePathBuilder();
            str.append("<div><a href=\"" + uri.path("" + (i + 1)).build() + "/\">" + e.getMatchers().get(i).toString() + "</a></div>\n");
        }

        uri = uriInfo.getAbsolutePathBuilder();
        str.append("<div><a href=\"" + uri.path("" + e.getViews().size()).build() + "/\">final</a></div>\n");
        return str.toString();
    }

    private SpectrumModel getSpectrum (String specId,
                                       int viewId) {
        Map<String, SpectrumEntry> entries = db.getSpectra();
        SpectrumEntry e = entries.get(specId);

        if (e == null)
            throw new NotFoundException("Invalid Spectrum Id");


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
    @ApiOperation(value = "/{specId}/{viewId}",
                  notes = "Retrieves a spectrum view.",
                  response = SpectrumModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid spectrum/view Ids.")})
    @Produces("application/json")
    @Path("/{specId}/{viewId}")
    public String sendSpectrum (@ApiParam(value = "The spectrum id.") @PathParam("specId") String specId,
                                @ApiParam(value = "The view id.") @PathParam("viewId") int viewId) {
        return json.deepSerialize(getSpectrum(specId, viewId));
    }

    @GET
    @Path("/{specId}/{viewId}/tree")
    @ApiOperation(value = "/{specId}/{viewId}/tree",
                  notes = "Retrieves the tree for a spectrum view.", response = SpectrumModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid spectrum/view Ids.")})
    @Produces("application/json")
    public String sendTree (@ApiParam(value = "The spectrum id.") @PathParam("specId") String specId,
                            @ApiParam(value = "The view id.") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(specId, viewId);


        return json.deepSerialize(s.getTree());
    }

    @GET
    @Path("/{specId}/{viewId}/probes")
    @ApiOperation(value = "/{specId}/{viewId}/probes",
                  notes = "Retrieves the tree for a spectrum view.", response = Iterable.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid spectrum/view Ids.")})
    @Produces("application/json")
    public String sendProbes (@ApiParam(value = "The spectrum id.") @PathParam("specId") String specId,
                              @ApiParam(value = "The view id.") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(specId, viewId);


        return json.deepSerialize(s.getProbes());
    }

    @GET
    @Path("/{specId}/{viewId}/transactions")
    @ApiOperation(value = "/{specId}/{viewId}/transactions", notes = "Retrieves the transactions for a spectrum view.", response = SpectrumModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid spectrum/view Ids.")})
    @Produces("application/json")
    public String sendTransactions (@ApiParam(value = "The spectrum id.") @PathParam("specId") String specId,
                                    @ApiParam(value = "The view id.") @PathParam("viewId") int viewId) {
        SpectrumModel s = getSpectrum(specId, viewId);


        return json.deepSerialize(s.getTransactions());
    }
}