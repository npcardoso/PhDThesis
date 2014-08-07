package io.crowbar.util;

import java.util.List;

public interface EditableFilter extends Filter {
    void filter (int item);
    void filterAll (List<Integer> list);
}