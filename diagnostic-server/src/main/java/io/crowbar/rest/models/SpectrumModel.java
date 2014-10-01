package io.crowbar.rest.models;

import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Tree;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

import java.util.List;
import flexjson.JSON;

@ApiModel(value = "Spectrum",
          description = "Spectrum resource representation")
public final class SpectrumModel {
    private final int sessionId;
    private final Integer parentId;
    private final String matcherName;
    private final Spectrum original;

    public SpectrumModel (int sessionId,
                          Spectrum original) {
        this(sessionId, null, null, original);
    }

    public SpectrumModel (int sessionId,
                          Integer parentId,
                          String matcherName,
                          Spectrum original) {
        this.sessionId = sessionId;
        this.parentId = parentId;
        this.matcherName = matcherName;
        this.original = original;
    }

    @ApiModelProperty(value = "The session's id",
                      required = true,
                      dataType = "integer")
    public int getSessionId () {
        return sessionId;
    }

    @ApiModelProperty(value = "The parent spectrum's id. If defined, this spectrum is a view of another spectrum.",
                      required = false)
    public Integer getParentId () {
        return parentId;
    }

    @ApiModelProperty(value = "The name of the matcher used to create this view.",
                      required = false)
    public String getMatcherName () {
        return matcherName;
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

    @ApiModelProperty(hidden = true)
    @JSON(include = false)
    public Spectrum getOriginal () {
        return original;
    }
}