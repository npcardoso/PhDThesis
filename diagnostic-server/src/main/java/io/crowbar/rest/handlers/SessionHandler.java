package io.crowbar.rest.handlers;


import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.rest.database.Database;
import io.crowbar.rest.database.SessionEntry;
import io.crowbar.rest.models.ApiResponseModel;
import io.crowbar.rest.models.IndexListModel;

import com.wordnik.swagger.annotations.*;
import java.util.Map;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.PathParam;


@Path("/sessions")
@Produces("application/json")
@Api(value = "/sessions", description = "Operations on sessions")
public final class SessionHandler {
    private final Database db;

    public SessionHandler (Database db) {
        this.db = db;
    }

    private SessionEntry getSessionPvt (int sessionId) {
        SessionEntry e = db.getSessions().get(sessionId);


        if (e == null)
            throw new NotFoundException("Invalid session Id");


        return e;
    }

    @GET
    @ApiOperation(value = "/",
                  notes = "Retrieves the list of session ids.",
                  response = Integer.class)
    public ApiResponseModel<IndexListModel> getSessionIds () {
        IndexListModel il = new IndexListModel(db.getSessions().elements().size());


        return new ApiResponseModel<IndexListModel> (il);
    }

    @GET
    @Path("/{sessionId}")
    @ApiOperation(value = "/{sessionId}",
                  notes = "Retrieves a session.",
                  response = SessionEntry.class)
    @ApiResponses({@ApiResponse(code = 404, message = "Invalid session Id.")})
    public ApiResponseModel<SessionEntry> getSession (@ApiParam(value = "The session's id") @PathParam("sessionId") int sessionId) {
        return new ApiResponseModel<SessionEntry> (getSessionPvt(sessionId));
    }
}