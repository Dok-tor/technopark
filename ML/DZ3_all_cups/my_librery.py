import re

import emoji
import nltk
import unicodedata
import contractions
from nltk import WordNetLemmatizer
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from sklearn.metrics import f1_score

STOP_WORDS = nltk.corpus.stopwords.words('russian') + nltk.corpus.stopwords.words('english')


def preprocess_news(lst):
    '''Function to preprocess and create corpus'''
    new_corpus=[]

    lem=WordNetLemmatizer()
    for text in lst:
        words=[w for w in word_tokenize(text) if (w not in STOP_WORDS)]
        words=[lem.lemmatize(w) for w in words]
        one_string = ' '.join(words)
        new_corpus.append(one_string)
    return new_corpus

# Функция для очистки текста
def clean_text(input_text):

    # HTML-теги: первый шаг - удалить из входного текста все HTML-теги
    clean_text = re.sub('<[^<]+?>', '', input_text)

    # URL и ссылки: далее - удаляем из текста все URL и ссылки
    clean_text = re.sub(r'http\S+', '', clean_text)

    # Эмоджи и эмотиконы: используем собственную функцию для преобразования эмоджи в текст
    # Важно понимать эмоциональную окраску обрабатываемого текста
    clean_text = emojis_words(clean_text)

    # Приводим все входные данные к нижнему регистру
    clean_text = clean_text.lower()

    stop_words = set(stopwords.words('russian'))
    tokens = word_tokenize(clean_text, language='russian')
    tokens = [token for token in tokens if token not in stop_words]
    clean_text = ' '.join(tokens)

    stop_words = set(stopwords.words('english'))
    tokens = word_tokenize(clean_text, language='english')
    tokens = [token for token in tokens if token not in stop_words]
    clean_text = ' '.join(tokens)

    # Убираем лишние пробелы и знаки препинания
    clean_text = re.sub('\s+', ' ', clean_text)
    clean_text = re.sub(r'[^\w\s]', '', clean_text)

    return clean_text

def clean_url(input_text):

    # HTML-теги: первый шаг - удалить из входного текста все HTML-теги
    # clean_text = re.sub('<[^<]+?>', '', input_text)

    # URL и ссылки: далее - удаляем из текста все URL и ссылки
    # clean_text = re.sub(r'http\S+', '', clean_text)

    # Эмоджи и эмотиконы: используем собственную функцию для преобразования эмоджи в текст
    # Важно понимать эмоциональную окраску обрабатываемого текста
    clean_text = emojis_words(input_text)

    # Приводим все входные данные к нижнему регистру
    clean_text = clean_text.lower()

    stop_words = set(stopwords.words('english'))
    tokens = word_tokenize(clean_text, language='english')
    tokens = [token for token in tokens if token not in stop_words]
    clean_text = ' '.join(tokens)

    # Убираем лишние пробелы и знаки препинания
    clean_text = re.sub('\s+', ' ', clean_text)
    clean_text = re.sub(r'[^\w\s]', ' ', clean_text)

    return clean_text

def clean_text_unicode(input_text):

    # HTML-теги: первый шаг - удалить из входного текста все HTML-теги
    clean_text = re.sub('<[^<]+?>', '', input_text)

    # URL и ссылки: далее - удаляем из текста все URL и ссылки
    clean_text = re.sub(r'http\S+', '', clean_text)

    # Эмоджи и эмотиконы: используем собственную функцию для преобразования эмоджи в текст
    # Важно понимать эмоциональную окраску обрабатываемого текста
    clean_text = emojis_words(clean_text)

    # Приводим все входные данные к нижнему регистру
    clean_text = clean_text.lower()

    # Убираем все пробелы
    # Так как все данные теперь представлены словами - удалим пробелы
    clean_text = re.sub('\s+', ' ', clean_text)
    clean_text = re.sub(r'[^\w\s]', '', clean_text)

    # Преобразование символов с диакритическими знаками к ASCII-символам: используем функцию normalize из модуля unicodedata и преобразуем символы с диакритическими знаками к ASCII-символам
    # clean_text = unicodedata.normalize('NFKD', clean_text).encode('ascii', 'ignore').decode('utf-8', 'ignore')
    clean_text = clean_text_keep_non_ascii(clean_text)
    # Разворачиваем сокращения: текст часто содержит конструкции вроде "don't" или "won't", поэтому развернём подобные сокращения
    clean_text = contractions.fix(clean_text)

    # Убираем специальные символы: избавляемся от всего, что не является "словами"
    # clean_text = re.sub('[^a-zA-Z0-9\s]', '', clean_text)

    # Записываем числа прописью: 100 превращается в "сто" (для компьютера)
    # temp = inflect.engine()
    # words = []
    # for word in clean_text.split():
    #     if word.isdigit():
    #         words.append(temp.number_to_words(word))
    #     else:
    #         words.append(word)
    # clean_text = ' '.join(words)

    # Стоп-слова: удаление стоп-слов - это стандартная практика очистки текстов
    stop_words = set(stopwords.words('russian'))
    tokens = word_tokenize(clean_text, language='russian')
    tokens = [token for token in tokens if token not in stop_words]
    clean_text = ' '.join(tokens)

    stop_words = set(stopwords.words('english'))
    tokens = word_tokenize(clean_text, language='english')
    tokens = [token for token in tokens if token not in stop_words]
    clean_text = ' '.join(tokens)

    # Знаки препинания: далее - удаляем из текста все знаки препинания


    # И наконец - возвращаем очищенный текст
    return clean_text

# Функция для преобразования эмоджи в слова
def emojis_words(text):

    # Модуль emoji: преобразование эмоджи в их словесные описания
    clean_text = emoji.demojize(text, delimiters=(" ", " "))

    # Редактирование текста путём замены ":" и" _", а так же - путём добавления пробела между отдельными словами
    clean_text = clean_text.replace(":", "").replace("_", " ")

    return clean_text

def clean_text_keep_non_ascii(text):
    # Нормализация текста в форму NFKD
    normalized_text = unicodedata.normalize('NFKD', text)

    # Преобразование в байтовую строку с использованием ASCII, сохраняя символы, которые нельзя представить в ASCII
    encoded_text = normalized_text.encode('ascii', 'backslashreplace')

    # Декодирование байтовой строки обратно в Unicode с использованием UTF-8
    decoded_text = encoded_text.decode('utf-8', 'backslashreplace')

    return decoded_text

def clean_data_list(d_list):
    l = []
    for string in d_list:
        l.append(clean_text(string))
    return l

def clean_data_list_unicode(d_list):
    l = []
    for string in d_list:
        l.append(clean_text_unicode(string))
    return l

def clean_url_data_list(d_list):
    l = []
    for string in d_list:
        l.append(clean_url(string))
    return l

def preprocess_news_after_clear_data(X):
    X = clean_data_list(X)
    X = preprocess_news(X)
    return X

def fit_predict(model, X_train, Y_train, X_test=None, Y_test=None):
    model.fit(X_train, Y_train)
    if (X_test is not None and Y_test is not None):
        return {
            'train' : f1_score(Y_train, model.predict(X_train)),
            'test' : f1_score(Y_test, model.predict(X_test))
        }
    return {'train' : f1_score(Y_train, model.predict(X_train))}

