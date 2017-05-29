import numpy as np
import tensorflow as tf
import math as math

num_inputs = 292
num_outputs = 4
nodes_1 = 512

entropy_graph = tf.Graph()

with entropy_graph.as_default():  
  # Variables.
  weights_1 = tf.Variable(
    tf.truncated_normal([num_inputs, nodes_1], stddev=0.01))
  biases_1 = tf.Variable(tf.zeros([nodes_1]))
  weights_4 = tf.Variable(
    tf.truncated_normal([nodes_1, num_outputs], stddev=0.01))
  biases_4= tf.Variable(tf.zeros([1]))

  forwards_input = tf.placeholder(tf.float32, shape=(None, num_inputs))
  mat_1 = tf.matmul(forwards_input, weights_1) + biases_1
  rel_1 = tf.nn.relu(mat_1)
  forwards_pred = tf.nn.softmax(tf.matmul(rel_1, weights_4) + biases_4)

session = tf.Session(graph=entropy_graph)
with entropy_graph.as_default():  
    tf.train.Saver().restore(session,"C:/Users/TomEccles/Documents/Visual Studio 2015/Projects/2048/2048/05_25_predictMoves.ckpt")
print("Initialized")

def compare(board):
    return [  (1 if board[i] > board[j] and board[j] > 0 else 0 )for i in range(16) for j in range(16)]

def go(a):
    b = np.concatenate([a, compare(a)]);
    p = session.run([forwards_pred], {forwards_input: [b]})
    return p[0][0]

print(go([5,3,1,0,2,3,0,0,1,2,0,0,1,0,0,0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,0,1,1,0,1]))
          
          
          
          
