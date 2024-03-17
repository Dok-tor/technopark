n = int(input())
max_arr = []
for i in range(n):
    max_arr.append(max(list(map(int, input().split()))))
print(";".join([str(x) for x in sorted(max_arr, reverse=True)]))
