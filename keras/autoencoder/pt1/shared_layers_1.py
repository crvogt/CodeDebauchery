import keras
from keras.layers import Input, LSTM, Dense
from keras.models import Model 
'''
This is an example of a symmetric problem where the
authors are attempting to compare users by the similarity
of their tweets

One way to achieve this is to build a model that encodes 
the two tweets into two vectors, concatenates the
vectors and then adds a logistic regression; this
outputs a probability that the two tweets share the
same author. The model would then be trained on positive 
tweet pairs and negative tweet pairs 

Because the problem is symmetric, the mechanism that encodes
the first tweet should be reused (weights and all) to encode the 
second tweet. Here we use a shared LSTM layer to encode the tweets.

We will build it with the functional API, and take as input 
for a tweet a binary matrix of shape (280, 256), ie a sequence 
of 280 vectors of size 256, where each dimension in the 256 d vector 
encodes the presence/absence of a character (outof an alphabet of 
	256 frequent characters)
'''

tweet_a = Input(shape=(280, 256))
tweet_b = Input(shape=(280, 256))

'''
To share a layer across different inputs, simply instantiate the layer 
once, then call it on as many inputs as you want:
'''

#This layer can take as input a matrix and will return a
#vector of size 64 
shared_lstm = LSTM(64)

#When we reuse the same layer instance multiple times,
#the weights of the layer are also being reused
#(it is effectively "the same" layer)
encoded_a = shared_lstm(tweet_a)
encoded_b = shared_lstm(tweet_b)

#We can then concatenate the two vectors:
merged_vector = keras.layers.concatenate([encoded_a, encoded_b], axis=-1)

#And add a logistic regression on top
predictions = Dense(1, activation='sigmoid')(merged_vector)

#We define a trainable model linking the 
#tweet inputs to the predictions
model = Model(inputs=[tweet_a, tweet_b], outputs=predictions)

model.compile(optimizer='rmsprop',
			  loss='binary_crossentropy', 
			  metrics=['accuracy'])
model.fit([data_a, data_b], labels, epochs=10)		

