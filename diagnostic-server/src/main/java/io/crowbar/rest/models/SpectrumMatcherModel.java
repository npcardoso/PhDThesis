package io.crowbar.rest.models;

import io.crowbar.diagnostic.spectrum.matchers.SpectrumMatcher;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

@ApiModel(value = "SpectrumMatcher",
          description = "Spectrum matcher representation")
public final class SpectrumMatcherModel {
    private final SpectrumMatcher original;

    public SpectrumMatcherModel (SpectrumMatcher original) {
        this.original = original;
    }

    @ApiModelProperty(value = "The SpectrumMatcher's name", required = true)
    public String getName () {
        return original.toString();
    }
}