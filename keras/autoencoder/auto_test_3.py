#Deep autoencoder
from keras.layers import Input, Dense
from keras.models import Model 

from keras.datasets import mnist 
import numpy as np 

import matplotlib.pyplot as plt 

#we'll normalize all values between 0 and 1 and flatten
#the 28x28 images into vectors of size 784
(x_train, _), (x_test, _) = mnist.load_data()

x_train = x_train.astype('float32')/255.
x_test = x_test.astype('float32')/255.
#I think the below can be replaced with "flatten()"
x_train = x_train.reshape((len(x_train), np.prod(x_train.shape[1:])))
x_test = x_test.reshape((len(x_test), np.prod(x_test.shape[1:])))

print x_train.shape
print x_test.shape

input_img = Input(shape=(784,))
encoded = Dense(128, activation='relu')(input_img)
encoded = Dense(64, activation='relu')(encoded)
encoded = Dense(32, activation='relu')(encoded)

decoded = Dense(64, activation='relu')(encoded)
decoded = Dense(128, activation='relu')(decoded)
decoded = Dense(784, activation='sigmoid')(decoded)

autoencoder = Model(input_img, decoded)
autoencoder.compile(optimizer='adadelta', loss='binary_crossentropy')

autoencoder.fit(x_train, x_train,
				epochs=100,
				batch_size=256,
				shuffle=True,
				validation_data=(x_test, x_test))

decoded_imgs = autoencoder.predict(x_test)

#How many digits we will display
n = 10 
plt.figure(figsize=(20, 4))
for i in range(n):
	#display original
	ax = plt.subplot(2, n, i + 1)
	plt.imshow(x_test[i].reshape(28, 28))
	plt.gray()
	ax.get_xaxis().set_visible(False)
	ax.get_yaxis().set_visible(False)

	#display reconstruction
	ax = plt.subplot(2, n, i + 1 + n)
	plt.imshow(decoded_imgs[i].reshape(28, 28))
	plt.gray()
	ax.get_xaxis().set_visible(False)
	ax.get_yaxis().set_visible(False)
plt.show()