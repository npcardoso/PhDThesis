package io.crowbar.rest.models;

import java.util.Iterator;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

@ApiModel(value = "IdList",
          description = "List of ids representation")
public final class IdListModel {
    private final Iterable it;
    public IdListModel (Iterable<Integer> it) {
        this.it = it;
    }

    @ApiModelProperty(value = "The list of ids",
                      required = true,
                      dataType = "array[int]")
    public Iterable<Integer> getIds () {
        return it;
    }
}