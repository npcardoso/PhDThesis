package io.crowbar.diagnosis.spectra;

import java.util.Iterator;

public interface Transaction<A extends Activity> extends Iterable<A> {
    /*!
     * \brief Returns the activity of component "id".
     * Should *not* raise any exceptions if the activity does not exist but instead return null.
     */
    A get (int id);
    double getError ();
    double getConfidence ();

    Iterator<A> iterator ();

    int numActive ();
    int size ();
}