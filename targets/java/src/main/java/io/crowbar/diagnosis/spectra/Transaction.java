package io.crowbar.diagnosis.spectra;

import java.util.Iterator;

public abstract class Transaction<A extends Activity,
                                  TM extends Metadata>
implements Iterable<A> {
    /*!
     * \brief Returns the activity of component "id".
     * Should *not* raise any exceptions if the activity does not exist but instead return null.
     */

    Transaction () {}

    public abstract A get (int id);

    public final boolean isError () {
        return getError() >= 1;
    };

    public abstract double getError ();

    public abstract double getConfidence ();

    public abstract TM getMetadata ();

    public abstract Iterator<A> iterator ();

    public abstract int numActive ();

    public abstract int size ();
}