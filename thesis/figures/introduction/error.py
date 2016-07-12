def f_to_c(temp_f):
    return (temp_f - 32) * 5 // 9

def is_freezing_c(temp_c):
    return temp_c <= 0

def is_freezing_f(temp_f):
    return is_freezing_c(f_to_c(temp_f))
