big_string = input()
punctuation = '!,.?;:#$%^&*()'
for char in punctuation:
    big_string = big_string.replace(char, '')
list_of_words = big_string.lower().split()

dictionary = {}
for word in list_of_words:
    if len(word) >= 5 and len(set(word)) >= 4:
        dictionary[word] = dictionary.get(word, 0) + 1

res_arr = [word for word in dictionary.keys() if dictionary[word] > 2]
for word in sorted(res_arr):
    print(word)
