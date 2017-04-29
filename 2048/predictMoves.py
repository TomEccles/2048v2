import numpy as np
import tensorflow as tf
import math as math

def accuracy_matrix(pred, labels):
    size = 4
    for i in range(size):
        for j in range(size):
            total = sum([a[i] for a in labels])
            
            matches = sum([a[j]*b[i] for a,b in zip(pred, labels)])
            print("%.3f" % (matches/total),end=",")
        print()

accuracy_matrix([[0,0,1,0],[0,0,0,1],[1,0,0,0],[1,0,0,0],[0,0,0,1]], [[0,0,1,0],[0,0,1,0],[0,1,0,0],[1,0,0,0],[0,0,0,1]])


file = 'moves.txt'
num_labels = 4
print(math.log(3))

with open(file, 'rb') as f:
  arr = np.loadtxt(f, delimiter=',')

positions = arr[:,:-1]
print(positions.shape)
print(positions[100])

moves = arr[:,-1]
moves = (np.arange(num_labels) == moves[:,None]).astype(np.float32)

batch_size = 128
nodes_1 = 512
nodes_2 = 128
nodes_3 = 32
num_inputs = positions.shape[2]
num_outputs = 4


valid_p = positions[-10000:,:].astype(np.float32)
valid_m = moves[-10000:,:].astype(np.float32)
positions = positions[:-10000,:].astype(np.float32)
moves = moves[:-10000,:].astype(np.float32)

print(positions[0])
print(moves[0])
print(positions.shape)
print(moves.shape)


l=0
for i in range(4):
    print(i, ": ", sum(valid_m[:,i]))
    prop = sum(valid_m[:,i]) / valid_m.shape[0]
    print (prop)
    l += -prop * math.log(prop)
print("Naive loss on validation set: ", l)

entropy_graph = tf.Graph()

def permute(dataset, labels):
  permutation = np.random.permutation(dataset.shape[0])
  shuffled_dataset = dataset[permutation,:]
  shuffled_labels = labels[permutation,:]
  return shuffled_dataset, shuffled_labels

permute(positions, moves)

def calc(x, keep_param = 1):
  mat_1 = tf.matmul(x, weights_1) + biases_1
  rel_1 = tf.nn.relu(mat_1)
  mat_2 = tf.matmul(rel_1, weights_2) + biases_2
  rel_2 = tf.nn.relu(mat_2)
  mat_3 = tf.matmul(rel_2, weights_3) + biases_3
  rel_3 = tf.nn.relu(mat_3)
  drop_3 = tf.nn.dropout(mat_3, keep_param)
  mat_4 = tf.matmul(drop_3, weights_4) + biases_4
  #final = tf.nn.sigmoid(mat_4)
  return mat_4

with entropy_graph.as_default():
  X = tf.placeholder(tf.float32, shape=(batch_size, num_inputs))
  Y = tf.placeholder(tf.float32, shape=(batch_size, num_outputs))
  reg_param = tf.placeholder(tf.float32)
  keep_param = tf.placeholder(tf.float32)
  learning_rate = tf.placeholder(tf.float32)
  
  # Variables.
  weights_1 = tf.Variable(
    tf.truncated_normal([num_inputs, nodes_1], stddev=0.01))
  biases_1 = tf.Variable(tf.zeros([nodes_1]))
  weights_2 = tf.Variable(
    tf.truncated_normal([nodes_1, nodes_2], stddev=0.001))
  biases_2 = tf.Variable(tf.zeros([nodes_2]))
  weights_3 = tf.Variable(
    tf.truncated_normal([nodes_2, nodes_3], stddev=0.01))
  biases_3 = tf.Variable(tf.zeros([nodes_3]))
  weights_4 = tf.Variable(
    tf.truncated_normal([nodes_3, num_outputs], stddev=0.01))
  biases_4= tf.Variable(tf.zeros([1]))

  #validation
  tf_valid_dataset = tf.constant(valid_p)
  tf_valid_labels = tf.constant(valid_m)

  
  # Training computation.
  weight_loss = reg_param*(tf.nn.l2_loss(weights_1)+tf.nn.l2_loss(weights_2)+tf.nn.l2_loss(weights_3)+tf.nn.l2_loss(weights_4))
  logits = calc(X, keep_param)
  loss =  tf.reduce_mean(
    tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=Y))+weight_loss

  # Optimizer.
  optimizer = tf.train.AdamOptimizer(1e-4).minimize(loss)
  
  # Predictions for validation data.
  valid_calc = calc(tf_valid_dataset, 1)
  valid_pred = tf.nn.softmax(valid_calc)
  valid_loss = tf.reduce_mean(
    tf.nn.softmax_cross_entropy_with_logits(logits=valid_calc, labels=tf_valid_labels))

  # Run it forwards
  forwards_input = tf.placeholder(tf.float32, shape=(None, num_inputs))
  forwards_pred = calc(forwards_input)

def accuracy(predictions, labels):
  return (100.0 * np.sum(np.argmax(predictions, 1) == np.argmax(labels, 1))
          / predictions.shape[0])

num_steps = 30001
reg=0.03
lr = 0.03
for dropout in [1]:
    best = 0
    with tf.Session(graph=entropy_graph) as session:
      tf.initialize_all_variables().run()
      print("Initialized [reg, drop, learn]", reg, dropout, lr)
      for step in range(num_steps):
        # Pick an offset within the training data, which has been randomized.
        # Note: we could use better randomization across epochs.
        offset = (step * batch_size) % (moves.shape[0] - batch_size)
        # Generate a minibatch.
        batch_data = positions[offset:(offset + batch_size), :]
        batch_labels = moves[offset:(offset + batch_size), :]
        # Prepare a dictionary telling the session where to feed the minibatch.
        # The key of the dictionary is the placeholder node of the graph to be fed,
        # and the value is the numpy array to feed to it.
        feed_dict = {X : batch_data, Y : batch_labels, reg_param : reg, keep_param : dropout, learning_rate : lr}
        _, l = session.run( [optimizer, loss], feed_dict=feed_dict )
        if (step % 1000 == 0):
         print("step %i loss: %f" % (step,l), end=",")
        if (step % 5000 == 0):
          print()
          l, pred = session.run( [valid_loss, valid_pred] )
          a = accuracy(pred, valid_m)
          print("Validation accuracy: %.1f%%" % a)
          print("Validation loss: %.4f" % l)
          accuracy_matrix(pred, valid_m)
          
          
