def my_map(func, seq):
    for val in seq:
        yield func(val)


func_in, seq_in = eval(input()), eval(input())

for x in my_map(func_in, seq_in):
    print(x)
