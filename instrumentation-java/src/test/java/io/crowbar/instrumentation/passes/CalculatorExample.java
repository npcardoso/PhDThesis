package io.crowbar.instrumentation.passes;

public class CalculatorExample {
    private String name;

    public CalculatorExample (String name) {
        this.name = name;
    }

    public String getName () {
        return this.name;
    }

    public int add (int x,
                    int y) {
        return x + y;
    }

    public int sub (int x,
                    int y) {
        return x - y;
    }

    public int mul (int x,
                    int y) {
        return x * y;
    }

    public double div (int x,
                       int y) {
        return ((double) x) / ((double) y);
    }
}