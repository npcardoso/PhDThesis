package io.crowbar.rest.models;

import io.crowbar.diagnostic.Diagnostic;
import io.crowbar.diagnostic.DiagnosticElement;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;
import java.util.AbstractList;
import java.util.List;


@ApiModel(value = "Node",
          description = "Node resource representation")
public final class DiagnosticModel {
    private final Diagnostic original;

    public DiagnosticModel (Diagnostic original) {
        this.original = original;
    }

    @ApiModelProperty(value = "The diagnostic's elements", required = true)
    public List<DiagnosticElementModel> getElements () {
        return new AbstractList<DiagnosticElementModel> () {
                   @Override
                   public int size () {
                       return original.size();
                   }

                   @Override
                   public DiagnosticElementModel get (int i) {
                       DiagnosticElement de = original.get(i);


                       if (de == null)
                           return null;

                       return new DiagnosticElementModel(de);
                   }
        };
    }
}