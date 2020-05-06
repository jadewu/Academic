# In this file, I calculate TF-IDF and cosine similarity
# using formulas in lecture notes.

# Read files
import re

def read_file(text):
    M = []
    with open(text) as f:
        for line in f.readlines():
            for word in line.split():
                word = re.findall('[A-Za-z]+', word)
                if word:
                    M.append(word[0].lower())
#    print(M)
    return M

words1 = read_file('d1.txt')
words2 = read_file('d2.txt')
words3 = read_file('d3.txt')
words4 = read_file('d4.txt')
words5 = read_file('d5.txt')
wordsq = read_file('d_query.txt')

all_words = words1 + words2 + words3 + words4 + words5 + wordsq
vocabulary = []

for i in all_words:
    if i not in vocabulary:
        vocabulary.append(i)
#print(vocabulary)

def vectorize(words):
    vector = [0]*len(vocabulary)
    for word in words:
        for i in range(len(vocabulary)):
            if vocabulary[i] == word:
                vector[i] += 1
                break
#    print(vector)
    return vector

vector1 = vectorize(words1)
vector2 = vectorize(words2)
vector3 = vectorize(words3)
vector4 = vectorize(words4)
vector5 = vectorize(words5)
vectorq = vectorize(wordsq)

    
# Compute tf-idf vectors
import math

vectors = [vector1, vector2, vector3, vector4, vector5, vectorq]
count = [0]*len(vocabulary)
for i in range(len(vector1)):
    for vector in vectors:
        if vector[i]:
            count[i] += 1
#print(count)
            
def tfidf(vector):
    tfidf = [0]*len(vocabulary)
    for i in range(len(vector)):
        idf = math.log(6/count[i])
        tfidf[i] = vector[i]*idf
    print(tfidf)
    return tfidf

tfidf1 = tfidf(vector1)
tfidf2 = tfidf(vector2)
tfidf3 = tfidf(vector3)
tfidf4 = tfidf(vector4)
tfidf5 = tfidf(vector5)
tfidfq = tfidf(vectorq)

# Compute Cosine Similarity

def cos(v1, v2):
    over = sum(p*q for p,q in zip(v1, v2))
    below = math.sqrt(sum([val**2 for val in v1])) * math.sqrt(sum([val**2 for val in v2]))
    if not below:
        return 0
    return over/below

similarity = [cos(tfidf1,tfidfq), cos(tfidf2,tfidfq), cos(tfidf3,tfidfq), cos(tfidf4,tfidfq), cos(tfidf5,tfidfq)]
print(similarity)

# Only the forth value in cosine similarity is not 0, and it is 0.1967.
# Therefore the file with maximum similarity value is d4.





