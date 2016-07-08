package io.crowbar.rest.models;

import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.ProbeType;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

@ApiModel(value = "Probe",
          description = "Probe resource representation")
public final class ProbeModel {
    private final Probe original;
    public ProbeModel (Probe original) {
        this.original = original;
    }

    @ApiModelProperty(value = "Probe type",
                      required = true)
    public ProbeType getType () {
        return original.getType();
    }

    @ApiModelProperty(value = "Node in the tree where the probe was placed",
                      required = true,
                      dataType = "integer")
    public int getNodeId () {
        return original.getNodeId();
    }
}