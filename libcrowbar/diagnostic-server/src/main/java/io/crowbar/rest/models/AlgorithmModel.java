package io.crowbar.rest.models;

import io.crowbar.diagnostic.algorithms.Algorithm;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;
import java.util.Map;

@ApiModel(value = "Algorithm",
          description = "Algorithm configuration representation")
public final class AlgorithmModel {
    private final Algorithm original;

    public AlgorithmModel (Algorithm original) {
        this.original = original;
    }

    @ApiModelProperty(value = "The algorithm's name", required = true)
    public String getName () {
        return original.getName();
    }

    @ApiModelProperty(value = "The algorithm's configurations", required = false)
    public Map<String, String> getConfigs () {
        return original.getConfigs();
    }
}