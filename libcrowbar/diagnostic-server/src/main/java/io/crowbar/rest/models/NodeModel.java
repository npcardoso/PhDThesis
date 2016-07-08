package io.crowbar.rest.models;

import io.crowbar.diagnostic.spectrum.Node;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

@ApiModel(value = "Node",
          description = "Node resource representation")
public final class NodeModel {
    private final Node original;
    public NodeModel (Node original) {
        this.original = original;
    }

    @ApiModelProperty(value = "The node's name with a prefix representing the node type",
                      required = true,
                      dataType = "string")
    public String getN () {
        return original.getNameWithSymbol();
    }

    @ApiModelProperty(value = "The node's parent Id",
                      required = false,
                      dataType = "int")
    public int getP () {
        return original.getParentId();
    }
}