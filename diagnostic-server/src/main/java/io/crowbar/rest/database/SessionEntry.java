package io.crowbar.rest.database;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;
import java.util.Date;

@ApiModel(value = "Session",
          description = "Session representation")
public final class SessionEntry {
    private final String globalId;
    private final Date d;

    public SessionEntry (String globalId) {
        d = new Date();
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
}