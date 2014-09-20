package io.crowbar.instrumentation.spectrum;

import io.crowbar.diagnostic.spectrum.HitTransaction;


public final class HitTransactionWithException
extends HitTransaction {
    private final String exceptionClass;
    private final String exceptionMessage;

    public HitTransactionWithException (int id,
                                        double error,
                                        double confidence,
                                        String exceptionClass,
                                        String exceptionMessage) {
        super(id, error, confidence);
        this.exceptionClass = exceptionClass;
        this.exceptionMessage = exceptionMessage;
    }

    public HitTransactionWithException (int id,
                                        boolean[] activity,
                                        double error,
                                        double confidence,
                                        String exceptionClass,
                                        String exceptionMessage) {
        super(id, activity, error, confidence);
        this.exceptionClass = exceptionClass;
        this.exceptionMessage = exceptionMessage;
    }

    public String getExceptionClass () {
        return exceptionClass;
    }

    public String getExceptionMessage () {
        return exceptionMessage;
    }

    @Override
    public String toString () {
        StringBuilder str = new StringBuilder();


        str.append("{class='Transaction', ");
        str.append("id=" + getId() + ", ");
        str.append("activity=[");
        boolean first = true;

        for (Integer id : this) {
            if (!first)
                str.append(",");

            str.append(id.toString());
            first = false;
        }

        str.append("], ");
        str.append("error=" + getError() + ", ");
        str.append("confidence=" + getConfidence() + ", ");
        str.append("exceptionClass='" + getExceptionClass() + "', ");
        str.append("exceptionMessage='" + getExceptionMessage() + "'}");
        return str.toString();
    }
}