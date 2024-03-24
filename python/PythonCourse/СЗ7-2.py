for value in map(lambda x: x**2 if x % 2 != 0 else -x, range(*map(int, input().split()))):
    print(value)
