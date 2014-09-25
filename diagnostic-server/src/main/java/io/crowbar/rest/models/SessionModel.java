package io.crowbar.rest.models;

import io.crowbar.diagnostic.Connection;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;
import java.util.Date;

@ApiModel(value = "Session",
          description = "Session representation")
public final class SessionModel {
    private final Date d;
    private final int localId;
    private final String globalId;

    public SessionModel (int localId,
                         String globalId) {
        d = new Date();
        this.localId = localId;
        this.globalId = globalId;
    }

    @ApiModelProperty(value = "The session start date",
                      required = true)
    public Date getDate () {
        return d;
    }

    @ApiModelProperty(value = "The global session Id",
                      required = true)
    public String getGlobalId () {
        return globalId;
    }

    @ApiModelProperty(value = "The local session Id",
                      required = true)
    public int getLocalId () {
        return localId;
    }
}