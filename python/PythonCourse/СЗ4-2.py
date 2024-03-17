string = input().split()
print(round(sum([len(x) for x in string]) / len(string), 2))
