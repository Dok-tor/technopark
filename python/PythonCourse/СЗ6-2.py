def sredn(st: str):
    summ = 0
    numbers = st.strip().split()
    for number in numbers:
        summ += int(number)
    return round(summ / len(numbers), 2)


if __name__ == '__main__':
    strings = []
    while string := input():
        strings.append(string)
    for string in strings:
        print(sredn(string))
