package io.crowbar.diagnostic.spectrum.serializers;


import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.Spectrum;
import java.util.List;
import java.util.ArrayList;


public final class HitSpectrumTableSerializer {
    public interface TableStyle {
        String header (Probe p);
        String columnStart ();
        String headerStart ();
        String headerSep (int width);
        String paddingLeft (int columnWidth,
                            int textWidth);
        String paddingRight (int columnWidth,
                             int textWidth);
    }

    public static class OrgModeTableStyle
    implements TableStyle {
        @Override
        public String header (Probe p) {
            return "Probe " + p.getId();
        }

        @Override
        public String columnStart () {
            return "|";
        }

        @Override
        public String headerStart () {
            return "+";
        }

        @Override
        public String headerSep (int width) {
            StringBuilder sb = new StringBuilder();


            while (width-- > 0)
                sb.append("-");

            sb.append("+");
            return sb.toString();
        }

        @Override
        public String paddingLeft (int columnWidth,
                                   int textWidth) {
            StringBuilder sb = new StringBuilder();
            int width = (columnWidth - textWidth) / 2;


            while (width-- > 0)
                sb.append(" ");

            return sb.toString();
        }

        @Override
        public String paddingRight (int columnWidth,
                                    int textWidth) {
            boolean extraSpace = ((columnWidth - textWidth) % 2) == 1;


            return paddingLeft(columnWidth, textWidth) + (extraSpace ? " |" : "|");
        }
    }

    public static String serialize (Spectrum spectrum) {
        return HitSpectrumSerializer.serialize(spectrum, "\n");
    }

    public static String serialize (Spectrum spectrum,
                                    TableStyle ts) {
        StringBuilder ret = new StringBuilder();


        List<String> headers = new ArrayList<String> (spectrum.getProbeCount() + 2);
        int maxWidth = 11;

        for (Probe p : spectrum.byProbe()) {
            String tmp = ts.header(p);
            headers.add(tmp);
            maxWidth = Math.max(maxWidth, tmp.length());
        }

        headers.add("error");
        headers.add("confidence");


        ret.append(ts.columnStart());

        for (String tmp : headers) {
            ret.append(ts.paddingLeft(maxWidth, tmp.length()));
            ret.append(tmp);
            ret.append(ts.paddingRight(maxWidth, tmp.length()));
        }

        ret.append("\n");

        ret.append(ts.headerStart());

        for (int i = 0; i < headers.size(); i++) {
            ret.append(ts.headerSep(maxWidth));
        }

        ret.append("\n");


        int probeCount = spectrum.getProbeCount();

        for (Transaction t : spectrum.byTransaction()) {
            ret.append(serialize(t, probeCount, maxWidth, ts));
            ret.append("\n");
        }

        return ret.toString();
    }

    public static String serialize (Transaction transaction,
                                    int probeCount,
                                    int maxWidth,
                                    TableStyle ts) {
        StringBuilder ret = new StringBuilder();
        int i = 0;


        ret.append(ts.columnStart());

        if (transaction != null) {
            for (i = 0; i < transaction.size(); i++) {
                ret.append(ts.paddingLeft(maxWidth, 1));
                ret.append(transaction.isActive(i) ? "1" : "0");
                ret.append(ts.paddingRight(maxWidth, 1));
            }
        }

        for (; i < probeCount; i++) {
            ret.append(ts.paddingLeft(maxWidth, 1));
            ret.append("0");
            ret.append(ts.paddingRight(maxWidth, 1));
        }

        String error = "" + (transaction == null ? 0 : transaction.getError());
        String confidence = "" + (transaction == null ? 0 : transaction.getConfidence());

        ret.append(ts.paddingLeft(maxWidth, error.length()));
        ret.append(error);
        ret.append(ts.paddingRight(maxWidth, error.length()));
        ret.append(ts.paddingLeft(maxWidth, confidence.length()));
        ret.append(confidence);
        ret.append(ts.paddingRight(maxWidth, confidence.length()));


        return ret.toString();
    }
}