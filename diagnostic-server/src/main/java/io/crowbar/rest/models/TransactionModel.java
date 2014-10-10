package io.crowbar.rest.models;

import io.crowbar.diagnostic.spectrum.Transaction;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

@ApiModel(value = "Transaction",
          description = "Transaction resource representation")
public final class TransactionModel {
    private final Transaction original;
    public TransactionModel (Transaction original) {
        this.original = original;
    }

    @ApiModelProperty(value = "Error value for the transaction",
                      required = true,
                      allowableValues = "range[0, 1]")
    public double getError () {
        return original.getError();
    }

    @ApiModelProperty(value = "Confidence value for the error score",
                      required = true,
                      allowableValues = "range[0, 1]")
    public double getConfidence () {
        return original.getConfidence();
    }

    @ApiModelProperty(value = "List of active probes in the transaction",
                      required = true,
                      dataType = "array[integer]")
    public Iterable<Integer> getActivity () {
        return original.getActivity();
    }
}