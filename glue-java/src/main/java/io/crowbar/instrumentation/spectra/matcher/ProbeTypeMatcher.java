package io.crowbar.instrumentation.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import io.crowbar.diagnosis.spectra.matcher.AbstractSpectraMatcher;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.spectra.CmpM;


import java.util.BitSet;
import java.util.Set;
import java.util.HashSet;

public final class ProbeTypeMatcher extends AbstractSpectraMatcher<Activity, Metadata, CmpM> {
    private final Set<ProbeType> types = new HashSet<ProbeType> ();
    public ProbeTypeMatcher (ProbeType... types) {
        this(true, types);
    }

    public ProbeTypeMatcher (boolean defaultValueTransactions, ProbeType... types) {
        super(defaultValueTransactions, false);

        for (ProbeType t : types) {
            this.types.add(t);
        }
    }

    @Override
    public BitSet matchComponents (Spectra< ? extends Activity, ? extends Metadata, ? extends CmpM> spectra) {
        BitSet ret = new BitSet();


        for (int i = 0; i < spectra.getComponentCount(); i++) {
            ProbeType type = spectra.getComponent(i).getMetadata().getType();

            if (types.contains(type))
                ret.set(i);
            else
                ret.clear(i);
        }

        return ret;
    }
}
