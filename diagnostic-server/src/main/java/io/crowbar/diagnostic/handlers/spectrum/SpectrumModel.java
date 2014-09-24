package io.crowbar.diagnostic.handlers.spectrum;

import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Tree;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

import java.util.List;
import java.util.AbstractList;

@ApiModel(value = "Spectrum", description = "Spectrum resource representation")
public final class SpectrumModel {
    private final Spectrum original;

    public SpectrumModel (Spectrum original) {
        this.original = original;
    }

    @ApiModelProperty(value = "Number of transactions in the spectrum", required = false)
    public int getTransactionCount () {
        return original.getTransactionCount();
    }

    @ApiModelProperty(value = "Number of probes in the spectrum", required = false)
    public int getProbeCount () {
        return original.getProbeCount();
    }

    @ApiModelProperty(value = "Spectrum's transactions", required = true)
    public List<TransactionModel> getTransactions () {
        return new AbstractList<TransactionModel> () {
                   @Override
                   public int size () {
                       return original.getTransactions().size();
                   }

                   @Override
                   public TransactionModel get (int i) {
                       Transaction t = original.getTransactions().get(i);


                       if (t == null)
                           return null;

                       return new TransactionModel(t);
                   }
        };
    }

    @ApiModelProperty(value = "Spectrum's probes", required = true)
    public List<ProbeModel> getProbes () {
        return new AbstractList<ProbeModel> () {
                   @Override
                   public int size () {
                       return original.getProbes().size();
                   }

                   @Override
                   public ProbeModel get (int i) {
                       Probe p = original.getProbes().get(i);


                       if (p == null)
                           return null;

                       return new ProbeModel(p);
                   }
        };
    }

    @ApiModelProperty(value = "Spectrum's tree", required = true)
    public TreeModel getTree () {
        return new TreeModel(original.getTree());
    }
}