package io.crowbar.rest.models;

import io.crowbar.diagnostic.Connection;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

@ApiModel(value = "Connection",
          description = "Connection representation")
public final class ConnectionModel {
    private final Connection original;

    public ConnectionModel (Connection original) {
        this.original = original;
    }

    @ApiModelProperty(value = "The candidate generator id",
                      required = true)
    public int getFrom () {
        return original.getFrom();
    }

    @ApiModelProperty(value = "The candidate ranker id",
                      required = true)
    public int getTo () {
        return original.getTo();
    }
}