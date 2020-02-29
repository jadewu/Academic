# In this file, I use functions in sklearn packes to generate
# TF-IDF and Cosine Similarity directly.

f = open('d1.txt', 'r')
text1 = f.read()
f.close()

f = open('d2.txt', 'r')
text2 = f.read()
f.close()

f = open('d3.txt', 'r')
text3 = f.read()
f.close()

f = open('d4.txt', 'r')
text4 = f.read()
f.close()

f = open('d5.txt', 'r')
text5 = f.read()
f.close()

f = open('d_query.txt', 'r')
query = f.read()
f.close()

from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer  
from sklearn.metrics.pairwise import cosine_similarity

def Compute(text):
    vectorizer = CountVectorizer()  
    # Frequencies  
    X = vectorizer.fit_transform(text)  
    # All the words  
    word = vectorizer.get_feature_names()  
    print("Words: \n", word)    
    print("Frequency array: \n", X.toarray())  
      
    transformer = TfidfTransformer()  
    print(transformer)  
    # Compute TF-IDF  
    tfidf = transformer.fit_transform(X)    
    print("TF-IDF: \n", tfidf.toarray())
    return tfidf.toarray()

text = [text1,text2,text3,text4,text5, query]
FF = Compute(text)
print("Cosine similarity: \n", cosine_similarity(FF))

# In cosine similarity array, array[i][j] indicates the results by comparing
# item i with item j. Therefore, we focus on the last line, which indicates
# comparing each text file with query file. The largest value is 0.27282277.
# The document with maximum similarity value is d4.





