package io.crowbar.rest.models;

import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Tree;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

import java.util.List;
import flexjson.JSON;

@ApiModel(value = "SpectrumSummary",
          description = "SpectrumSummary resource representation")
public final class SpectrumSummaryModel {
    private final int sessionId;
    private final Spectrum original;

    public SpectrumSummaryModel (int sessionId,
                                 Spectrum original) {
        this.sessionId = sessionId;
        this.original = original;
    }

    @ApiModelProperty(value = "The session's id",
                      required = true,
                      dataType = "integer")
    public int getSessionId () {
        return sessionId;
    }

    @ApiModelProperty(value = "Number of transactions in the spectrum",
                      required = false,
                      dataType = "integer")
    public int getTransactionCount () {
        return original.getTransactionCount();
    }

    @ApiModelProperty(value = "Number of probes in the spectrum",
                      required = false,
                      dataType = "integer")
    public int getProbeCount () {
        return original.getProbeCount();
    }

    @ApiModelProperty(value = "Number of nodes in the spectrum's tree",
                      required = true,
                      dataType = "integer")
    public int getNodeCount () {
        return original.getTree().size();
    }

    @ApiModelProperty(value = "Average number of active probes per transaction",
                      required = true,
                      dataType = "double")
    public double getAverageActivationCount () {
        return original.getAverageActivationCount();
    }

    @ApiModelProperty(value = "Activation rate for the spectrum's probes",
                      required = true,
                      dataType = "double")
    public double getActivationRate () {
        return original.getActivationRate();
    }

    @ApiModelProperty(value = "Number of failing transactions in the spectrum",
                      required = true,
                      dataType = "integer")
    public int getErrorCount () {
        return original.getErrorCount(1);
    }

    @ApiModelProperty(value = "Number of passing transactions in the spectrum",
                      required = true,
                      dataType = "integer")
    public int getPassCount () {
        return original.getPassCount(1);
    }

    @ApiModelProperty(value = "Error rate for the spectrum's transactions",
                      required = true,
                      dataType = "double")
    public double getErrorRate () {
        return original.getErrorRate(1);
    }

    @ApiModelProperty(hidden = true)
    @JSON(include = false)
    public Spectrum getOriginal () {
        return original;
    }
}