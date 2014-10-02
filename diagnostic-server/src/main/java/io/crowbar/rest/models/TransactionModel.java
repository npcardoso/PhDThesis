package io.crowbar.rest.models;

import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.instrumentation.spectrum.HitTransactionWithException;

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

    @ApiModelProperty(value = "The test name",
                      required = false,
                      dataType = "string")
    public String getTestName () {
        Transaction t = original.getOriginal();


        if (t instanceof HitTransactionWithException)
            return ((HitTransactionWithException) t).getTestName();

        return null;
    }

    @ApiModelProperty(value = "The exception class",
                      required = false,
                      dataType = "string")
    public String getExceptionClass () {
        Transaction t = original.getOriginal();


        if (t instanceof HitTransactionWithException)
            return ((HitTransactionWithException) t).getExceptionClass();

        return null;
    }

    @ApiModelProperty(value = "The exception message",
                      required = false,
                      dataType = "string")
    public String getExceptionMessage () {
        Transaction t = original.getOriginal();


        if (t instanceof HitTransactionWithException)
            return ((HitTransactionWithException) t).getExceptionMessage();

        return null;
    }
}