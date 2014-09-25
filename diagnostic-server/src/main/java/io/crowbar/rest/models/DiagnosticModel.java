package io.crowbar.rest.models;

import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.DiagnosticElement;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;
import java.util.AbstractList;
import java.util.List;


@ApiModel(value = "Diagnostic",
          description = "Diagnostic resource representation")
public final class DiagnosticModel {
    private final Diagnostic original;

    public DiagnosticModel (Diagnostic original) {
        this.original = original;
    }

    @ApiModelProperty(value = "The diagnostic's elements", required = true)
    public List<DiagnosticElementModel> getElements () {
        ListWrapper.WrapperFactory<DiagnosticElement, DiagnosticElementModel> factory =
            new ListWrapper.WrapperFactory<DiagnosticElement, DiagnosticElementModel> () {
            @Override
            public DiagnosticElementModel create (DiagnosticElement original) {
                return new DiagnosticElementModel(original);
            }
        };

        return new ListWrapper<DiagnosticElement, DiagnosticElementModel> (original,
                                                                           factory);
    }
}