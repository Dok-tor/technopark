fib_dict = {}


def fibonachchi(n):
    if n == 0:
        return 0
    elif n == 1:
        return 1
    elif n in fib_dict.keys():
        return fib_dict[n]
    return fibonachchi(n-1) + fibonachchi(n-2)


if __name__ == '__main__':
    print(fibonachchi(int(input())))
