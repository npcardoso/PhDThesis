package io.crowbar.rest.models;

import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.DiagnosticElement;
import io.crowbar.diagnostic.DiagnosticReport;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;
import java.util.AbstractList;
import java.util.List;
import flexjson.JSON;
import java.util.LinkedList;


@ApiModel(value = "DiagnosticReport",
          description = "Diagnostic Report resource representation")
public final class DiagnosticReportModel {
    private final int spectrumId;
    private final int diagnosticSystemId;
    private final DiagnosticReport original;

    public DiagnosticReportModel (int spectrumId,
                                  int diagnosticSystemId,
                                  DiagnosticReport original) {
        this.spectrumId = spectrumId;
        this.diagnosticSystemId = diagnosticSystemId;
        this.original = original;
    }

    public int getSpectrumId () {
        return spectrumId;
    }

    public int getDiagnosticSystemId () {
        return diagnosticSystemId;
    }

    @ApiModelProperty(value = "The diagnostic's elements", required = true)
    public List<DiagnosticModel> getConnections () {
        ListWrapper.WrapperFactory<Diagnostic, DiagnosticModel> factory =
            new ListWrapper.WrapperFactory<Diagnostic, DiagnosticModel> () {
            @Override
            public DiagnosticModel create (Diagnostic original) {
                return new DiagnosticModel(original);
            }
        };
        // TODO: Refactor Diagnostic to contain connections
        return new ListWrapper<Diagnostic, DiagnosticModel> (new LinkedList<Diagnostic> (),
                                                             factory);
    }

    @JSON(include = false)
    public DiagnosticReport getOriginal () {
        return original;
    }
}