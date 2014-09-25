package io.crowbar.rest.models;

import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Tree;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

import java.util.List;

@ApiModel(value = "Spectrum",
          description = "Spectrum resource representation")
public final class SpectrumModel {
    private final Spectrum original;

    public SpectrumModel (Spectrum original) {
        this.original = original;
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

    @ApiModelProperty(value = "Spectrum's transactions",
                      required = true)
    public List<TransactionModel> getTransactions () {
        ListWrapper.WrapperFactory<Transaction, TransactionModel> factory =
            new ListWrapper.WrapperFactory<Transaction, TransactionModel> () {
            @Override
            public TransactionModel create (Transaction original) {
                return new TransactionModel(original);
            }
        };

        return new ListWrapper<Transaction, TransactionModel> (original.getTransactions(),
                                                               factory);
    }

    @ApiModelProperty(value = "Spectrum's probes",
                      required = true)
    public List<ProbeModel> getProbes () {
        ListWrapper.WrapperFactory<Probe, ProbeModel> factory =
            new ListWrapper.WrapperFactory<Probe, ProbeModel> () {
            @Override
            public ProbeModel create (Probe original) {
                return new ProbeModel(original);
            }
        };

        return new ListWrapper<Probe, ProbeModel> (original.getProbes(),
                                                   factory);
    }

    @ApiModelProperty(value = "Spectrum's tree",
                      required = true)
    public TreeModel getTree () {
        return new TreeModel(original.getTree());
    }
}