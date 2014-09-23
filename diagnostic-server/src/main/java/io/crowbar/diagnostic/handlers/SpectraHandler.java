package io.crowbar.diagnostic.handlers;


import io.crowbar.diagnostic.database.Database;
import io.crowbar.diagnostic.database.SpectrumEntry;
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


@Path("/spectra")
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

    @GET
    @Produces("application/json")
    @Path("/{specId}/{viewId}")
    public String sendSpectrum (@PathParam("specId") String specId,
                                @PathParam("viewId") int viewId) {
        Map<String, SpectrumEntry> entries = db.getSpectra();
        SpectrumEntry e = entries.get(specId);

        if (e == null)
            throw new NotFoundException();


        Spectrum s = null;

        if (viewId == 0)
            s = e.getOriginal();
        else if (viewId > 0)
            s = e.getViews().get(viewId - 1);

        if (s == null)
            throw new NotFoundException();

        return json.deepSerialize(s);
    }
}