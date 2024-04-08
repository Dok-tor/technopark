# Получение текстовой строки из списка слов
import nltk
from matplotlib import pyplot as plt
from wordcloud import WordCloud
STOP_WORDS = nltk.corpus.stopwords.words('russian') + nltk.corpus.stopwords.words('english')


def str_corpus(corpus):
    str_corpus = ''
    for i in corpus:
        str_corpus += ' ' + i
    str_corpus = str_corpus.strip()
    return str_corpus


# Получение списка всех слов в корпусе
def get_corpus(data):
    corpus = []
    for phrase in data:
        for word in phrase.split():
            corpus.append(word)
    return corpus
# Получение облака слов


def get_wordCloud(corpus):
    wordCloud = WordCloud(background_color='white',
                          stopwords=STOP_WORDS,
                          width=3000,
                          height=2500,
                          max_words=200,
                          random_state=42
                          ).generate(str_corpus(corpus))
    return wordCloud


def plot_word_cloud(data):
    corpus = get_corpus(data)
    procWordCloud = get_wordCloud(corpus)

    fig = plt.figure(figsize=(20, 8))
    plt.subplot(1, 2, 1)
    plt.imshow(procWordCloud)
    plt.axis('off')
    plt.subplot(1, 2, 1)