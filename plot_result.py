import matplotlib.pyplot as plt
import csv
import numpy
import Image

reader = csv.reader(open("Results/y_out.csv", "rb"),delimiter = ',')
x = list(reader)

y_out = numpy.array(x).astype('float')

reader = csv.reader(open("Data/y_test.csv", "rb"),delimiter = ',')
x = list(reader)

y_test = numpy.array(x).astype('float')

Plot1 = plt.plot(y_test,color = '#DE7A22', label = 'Y_test')
Plot2 = plt.plot(y_out,color ='#2C7873',label = 'Y_out')
plt.setp(Plot1, linewidth=3.0, aa=True, alpha=1)
plt.setp(Plot2, linewidth=3.0, aa=True, alpha= 1)
plt.ylabel('Target')
plt.xlabel('Input')
plt.legend()
plt.title('Extreme Learning Machine')
plt.grid(True)
plt.savefig('ELM_Results.jpg')
plt.show()

