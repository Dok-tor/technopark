string = input().lower()
print(string.count('!') + string.count('@') + string.count('#') + string.count('%'))
print(string.replace('!', '').replace('@', '').replace('#', '').replace('%', ''))
