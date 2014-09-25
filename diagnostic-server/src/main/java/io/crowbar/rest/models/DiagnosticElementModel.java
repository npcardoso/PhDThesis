package io.crowbar.rest.models;

import io.crowbar.diagnostic.DiagnosticElement;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;
import java.util.List;

@ApiModel(value = "DiagnosticElement",
          description = "DiagnosticElement representation")
public final class DiagnosticElementModel {
    private final DiagnosticElement original;

    public DiagnosticElementModel (DiagnosticElement original) {
        this.original = original;
    }

    @ApiModelProperty(value = "The element's score",
                      required = true)
    public double getS () {
        return original.getScore();
    }

    @ApiModelProperty(value = "The element's candidate",
                      required = true,
                      dataType = "array[integer]")
    public Iterable<Integer> getC () {
        return original.getCandidate();
    }
}