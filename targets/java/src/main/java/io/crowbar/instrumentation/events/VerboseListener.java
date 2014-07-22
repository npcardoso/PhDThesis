package io.crowbar.instrumentation.events;

public class VerboseListener implements EventListener {
    @Override
    public void startTransaction (int probe_id) {
        System.out.println("!!!!!!!!! transaction start @ " + probe_id + " !!!!!!!!!");
    }

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) {
        System.out.println("!!!!!!!!! transaction end @ " + probe_id + " !!!!!!!!!");

        for (boolean b : hit_vector)
            System.out.print(b ? "1 " : "0 ");

        System.out.println("");
    }

    @Override
    public void oracle (int probe_id,
                        double error,
                        double confidence) {
        System.out.println("!!!!!!!!! collecting oracle @ " + probe_id + " : " + error +
                           ", " + confidence + " !!!!!!!!!");
    }
}