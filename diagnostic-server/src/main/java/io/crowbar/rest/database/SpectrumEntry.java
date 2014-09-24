package io.crowbar.rest.database;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.SpectrumView;
import io.crowbar.diagnostic.spectrum.SpectrumViewFactory;
import io.crowbar.diagnostic.spectrum.matchers.SpectrumMatcher;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class SpectrumEntry {
    private final Spectrum original;
    private final List<SpectrumMatcher> matchers;
    private final List<SpectrumView> views;

    public Spectrum getOriginal () {
        return original;
    }

    public Spectrum getFinal () {
        if (views.size() == 0)
            return original;

        return views.get(views.size() - 1);
    }

    public List<SpectrumView> getViews () {
        return views;
    }

    public List<SpectrumMatcher> getMatchers () {
        return matchers;
    }

    public SpectrumEntry (Spectrum original,
                          List<SpectrumMatcher> matchers) {
        this.original = original;
        this.matchers = Collections.unmodifiableList(matchers);
        List<SpectrumView> tmp = new ArrayList<SpectrumView> ();
        SpectrumViewFactory svf = new SpectrumViewFactory(original);

        for (SpectrumMatcher m : matchers) {
            svf.addStage(m);
            tmp.add(svf.getView());
        }

        this.views = Collections.unmodifiableList(tmp);
    }
}