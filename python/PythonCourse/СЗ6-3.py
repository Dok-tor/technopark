def str_process(st: str):
    result = st.upper() if st[0] == '!' else st.lower()
    return result.replace('!', '').replace('@', '').replace('#', '').replace('%', '')


if __name__ == '__main__':
    strings = []
    while string := input():
        strings.append(string)
    for string in strings:
        print(str_process(string))
