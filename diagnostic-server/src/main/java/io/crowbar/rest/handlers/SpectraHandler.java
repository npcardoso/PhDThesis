package io.crowbar.rest.handlers;


import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.rest.database.Database;
import io.crowbar.rest.models.ApiResponseModel;
import io.crowbar.rest.models.IdListModel;
import io.crowbar.rest.models.SpectrumModel;
import io.crowbar.rest.models.SpectrumSummaryModel;
import io.crowbar.rest.models.TreeModel;
import io.crowbar.rest.models.TransactionModel;
import io.crowbar.rest.models.ProbeModel;
import io.crowbar.rest.models.IndexListModel;

import com.wordnik.swagger.annotations.*;
import java.util.Map;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.PathParam;
import java.util.List;


@Path("/spectra")
@Produces("application/json")
@Api(value = "/spectra", description = "Operations about spectra")
public final class SpectraHandler {
    private final Database db;

    public SpectraHandler (Database db) {
        this.db = db;
    }

    private SpectrumModel getSpectrumPvt (int spectrumId) {
        SpectrumModel e = db.getSpectra().get(spectrumId);


        if (e == null)
            throw new NotFoundException("Invalid spectrum Id");

        return e;
    }

    @GET
    @ApiOperation(value = "/",
                  notes = "Retrieves the list of spectrum ids.",
                  response = IndexListModel.class)
    public ApiResponseModel<IndexListModel> getSpectrumIds () {
        IndexListModel il = new IndexListModel(db.getSpectra().elements().size());


        return new ApiResponseModel<IndexListModel> (il);
    }

    @GET
    @Path("/{spectrumId}")
    @ApiOperation(value = "/{spectrumId}",
                  notes = "Retrieves a spectrum.",
                  response = SpectrumModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid spectrum Id.")})
    public ApiResponseModel<SpectrumModel> getSpectrum (@ApiParam(value = "The spectrum's id") @PathParam("spectrumId") int spectrumId) {
        SpectrumModel s = getSpectrumPvt(spectrumId);


        return new ApiResponseModel<SpectrumModel> (s);
    }

    @GET
    @Path("/{spectrumId}/tree")
    @ApiOperation(value = "/{spectrumId}/tree",
                  notes = "Retrieves the tree for a spectrum.",
                  response = TreeModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid spectrum Id.")})
    public ApiResponseModel<TreeModel> getTree (@ApiParam(value = "The spectrum's id") @PathParam("spectrumId") int spectrumId) {
        SpectrumModel s = getSpectrumPvt(spectrumId);


        return new ApiResponseModel<TreeModel> (s.getTree());
    }

    @GET
    @Path("/{spectrumId}/probes")
    @ApiOperation(value = "/{spectrumId}/probes",
                  notes = "Retrieves the probes for a spectrum.",
                  response = ProbeModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid spectrum Id.")})
    public ApiResponseModel<List<ProbeModel> > getProbes (@ApiParam(value = "The spectrum's id") @PathParam("spectrumId") int spectrumId) {
        SpectrumModel s = getSpectrumPvt(spectrumId);


        return new ApiResponseModel<List<ProbeModel> > (s.getProbes());
    }

    @GET
    @Path("/{spectrumId}/transactions")
    @ApiOperation(value = "/{spectrumId}/transactions",
                  notes = "Retrieves the transactions for a spectrum.",
                  response = TransactionModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid spectrum Id.")})
    public ApiResponseModel<List<TransactionModel> > getTransactions (@ApiParam(value = "The spectrum's id") @PathParam("spectrumId") int spectrumId) {
        SpectrumModel s = getSpectrumPvt(spectrumId);


        return new ApiResponseModel<List<TransactionModel> > (s.getTransactions());
    }

    @GET
    @Path("/{spectrumId}/summary")
    @ApiOperation(value = "/{spectrumId}/summary",
                  notes = "Retrieves the transactions for a spectrum.",
                  response = SpectrumSummaryModel.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid spectrum Id.")})
    public ApiResponseModel<SpectrumSummaryModel> getSummary (@ApiParam(value = "The spectrum's id") @PathParam("spectrumId") int spectrumId) {
        SpectrumModel s = getSpectrumPvt(spectrumId);
        SpectrumSummaryModel ss = new SpectrumSummaryModel(spectrumId, s.getOriginal());


        return new ApiResponseModel<SpectrumSummaryModel> (ss);
    }
}