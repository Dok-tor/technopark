big_string = input().lower().split()
d = dict()
for word in big_string:
    d[word] = d.get(word, 0) + 1

max_frequency = max(d.values())
for word in d.keys():
    if d[word] == max_frequency:
        print(word, max_frequency)
