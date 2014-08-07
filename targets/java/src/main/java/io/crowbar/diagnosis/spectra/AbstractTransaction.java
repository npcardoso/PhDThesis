package io.crowbar.diagnosis.spectra;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public abstract class AbstractTransaction<A extends Activity,
                                          TM extends Metadata>
implements Transaction<A, TM> {
    private final List<A> activity = new ArrayList<A> ();
    private final TM metadata;
    private final int active;
    private final double error;
    private final double confidence;


    public AbstractTransaction (A[] activity,
                                double error,
                                double confidence,
                                TM metadata) {
        int active = 0;


        for (A a: activity) {
            this.activity.add(a);
            active += a.isActive() ? 1 : 0;
        }

        this.active = active;
        this.error = error;
        this.confidence = confidence;
        this.metadata = metadata;
    }

    public AbstractTransaction (A[] activity,
                                double error,
                                double confidence) {
        this(activity, error, confidence, null);
    }

    public AbstractTransaction (List<A> activity,
                                double error,
                                double confidence,
                                TM metadata) {
        int active = 0;


        for (A a: activity) {
            this.activity.add(a);
            active += a.isActive() ? 1 : 0;
        }

        this.active = active;
        this.error = error;
        this.confidence = confidence;
        this.metadata = metadata;
    }

    public AbstractTransaction (List<A> activity,
                                double error,
                                double confidence) {
        this(activity, error, confidence, null);
    }

    @Override
    public final A get (int id) {
        if (id >= activity.size())
            return null;

        return activity.get(id);
    }

    @Override
    public final double getError () {
        return error;
    }

    @Override
    public final double getConfidence () {
        return confidence;
    }

    @Override
    public final TM getMetadata () {
        return metadata;
    }

    @Override
    public final Iterator<A> iterator () {
        return activity.iterator();
    }

    @Override
    public final int numActive () {
        return active;
    }

    @Override
    public final int size () {
        return activity.size();
    }
}