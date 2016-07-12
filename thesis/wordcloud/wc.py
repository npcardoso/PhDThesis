#!/usr/bin/env python2

from os import path
import matplotlib.pyplot as plt
import random
import math

from wordcloud import WordCloud, STOPWORDS

d = path.dirname(__file__)


allowed = " -"
text = open("main.txt").read()
text = [x  if (x.isalpha() or x in allowed) else " " for x in text]
text = "".join(text)

t = text.split()
t = [w for w in t if len(w)>2]
t = [w for w in t if w.isalpha()]

text = " ".join(t)

# adding movie script specific stopwords
stopwords = STOPWORDS.copy()
stopwords.add("page")
stopwords.add("pages")
stopwords.add("figure")
stopwords.add("see")
stopwords.add("section")
stopwords.add("cited")
stopwords.add("also")
stopwords.add("since")
stopwords.add("using")
stopwords.add("equation")
stopwords.add("must")
stopwords.add("based")
stopwords.add("abreu")
stopwords.add("kleer")
stopwords.add("van")
stopwords.add("gemund")


height = 1000
width = int(height / math.sqrt(2))

max_font_size=width/2

try:
    import sys
    seed = int(sys.argv[0])
except:
    seed = 0



wc = WordCloud(max_words=10000,
               stopwords=stopwords,
               margin=5,
               random_state=seed,
               height=height,
               max_font_size=max_font_size,
               width=width,
               prefer_horizontal=0.75)

freqs = wc.process_text(text)


freqs = [freqs[1::2],
         freqs[::2]]


count = [0, 0]



for i in range(min([len(x) for x in freqs])):
    if((len(freqs[0][i][0]) > len(freqs[1][i][0]) and count[0] > count[1]) or
       (len(freqs[0][i][0]) < len(freqs[1][i][0]) and count[0] < count[1])):
        freqs[0][i], freqs[1][i] = freqs[1][i], freqs[0][i]
        print("Swapped %d" % i, freqs[0][i], freqs[1][i], count)

    for x in range(2):
        count[x] += len(freqs[x][i][0])




def grey_color_func(word, font_size, position, orientation, random_state=None, **kwargs):
    grey_color_func.i += 1

    x = int(position[0] / width * 40)
    y = int(position[1] / height* 40)

    minl = 40 + \
           5 * (-3 + (x % 13 + y % 5 + y % 3 + x % 11) % 7 ) + \
           10 * (1 - (font_size/max_font_size)**2) + \
           20 * (-1 + (len(word) % 3  + len(word) % 2) % 3 ) + \
           20 * (-1 + (2 * int(i/2) % 2))
    return "hsl(0, 0%%, %d%%)" % random_state.randint(max(45, minl), 100)
grey_color_func.i = 0


for i in range(len(freqs)):
    wc = wc.generate_from_frequencies(freqs[i])

    default_colors = wc.to_array()

    # store default colored image
    plt.figure()
    plt.title("Custom colors")
    plt.imshow(wc.recolor(color_func=grey_color_func, random_state=3))
    wc.to_file("wordcloud%d.png" % i)
    plt.axis("off")
plt.show()
