import numpy as np
import tensorflow as tf
import math as math

def permute(dataset, labels):
  permutation = np.random.permutation(dataset.shape[0])
  shuffled_dataset = dataset[permutation,:]
  shuffled_labels = labels[permutation]
  return shuffled_dataset, shuffled_labels

file = '05_25_values.txt'
with open(file, 'rb') as f:
  arr = np.loadtxt(f, delimiter=',')


positions = arr[:,:-1]
moves = arr[:,-1]/1000


sums = np.array([[sum([2**i for i in a[:16] if i > 0])] for a in positions])
positions = np.concatenate([positions, sums],axis=1)
print(positions.shape)


def compare(board):
    return [  (1 if board[i] > board[j] and board[j] > 0 else 0 )for i in range(16) for j in range(16)]


print(positions[12])
print(compare(positions[12]))
positions = np.concatenate([positions, [compare(i) for i in positions]], axis = 1)
print(positions[12])
num_inputs = positions.shape[1]


valid_points = 30000
valid_p = positions[:valid_points,:].astype(np.float32)
valid_m = moves[:valid_points].astype(np.float32)
moves = moves[valid_points:].astype(np.float32)
positions = positions[valid_points:,:].astype(np.float32)
permute(positions, moves)


batch_size = 128
nodes_1 = 512
num_outputs = 1

mean = sum(valid_m)/len(valid_m)
loss = sum([(a-mean)*(a-mean) for a in valid_m])/len(valid_m)
print("Loss just from mean: ", loss)

differences = valid_m + valid_p[:,-1]/1000/2.2
print (differences[:20])
mean = sum(differences)/len(differences)
loss = sum([(a-mean)*(a-mean) for a in differences])/len(differences)
print("Loss from turns: ", loss)


entropy_graph = tf.Graph()
def calc(x, keep_param = 1):
  mat_1 = tf.matmul(x, weights_1) + biases_1
  rel_1 = tf.nn.relu(mat_1)
  drop_3 = tf.nn.dropout(rel_1, keep_param)
  mat_4 = tf.matmul(drop_3, weights_4) + biases_4
  return mat_4

with entropy_graph.as_default():
  X = tf.placeholder(tf.float32, shape=(batch_size, num_inputs))
  Y = tf.placeholder(tf.float32, shape=(batch_size))
  reg_param = tf.placeholder(tf.float32)
  keep_param = tf.placeholder(tf.float32)
  learning_rate = tf.placeholder(tf.float32)
  
  # Variables.
  weights_1 = tf.Variable(
    tf.truncated_normal([num_inputs, nodes_1], stddev=0.01))
  biases_1 = tf.Variable(tf.zeros([nodes_1]))
  weights_4 = tf.Variable(
    tf.truncated_normal([nodes_1, num_outputs], stddev=0.01))
  biases_4= tf.Variable(tf.zeros([num_outputs]))

  #validation
  tf_valid_dataset = tf.constant(valid_p)
  tf_valid_labels = tf.constant([[a] for a in valid_m])
  
  # Training computation.
  weight_loss = reg_param*(tf.nn.l2_loss(weights_1)+tf.nn.l2_loss(weights_4))
  predictions = calc(X, keep_param)
  
  diff = (predictions - Y)
  loss =  tf.reduce_mean(tf.nn.l2_loss(predictions - Y)) / batch_size+weight_loss

  # Optimizer.
  optimizer = tf.train.AdamOptimizer(1e-6).minimize(loss)
  
  # Predictions for validation data.
  valid_calc = calc(tf_valid_dataset, 1)
  diff_v = (valid_calc - tf_valid_labels)
  loss_v = tf.nn.l2_loss(diff_v)
  valid_loss = tf.reduce_mean(loss_v) / valid_points

  # Run it forwards
  forwards_input = tf.placeholder(tf.float32, shape=(None, num_inputs))
  forwards_pred = calc(forwards_input)

num_steps = 5000001
dropout = 0.2
reg = 0
best = None
with tf.Session(graph=entropy_graph) as session:
  tf.initialize_all_variables().run()
  for step in range(num_steps):
    # Pick an offset within the training data, which has been randomized.
    # Note: we could use better randomization across epochs.
    offset = (step * batch_size) % (moves.shape[0] - batch_size)
    # Generate a minibatch.
    batch_data = positions[offset:(offset + batch_size), :]
    batch_labels = moves[offset:(offset + batch_size)]
    # Prepare a dictionary telling the session where to feed the minibatch.
    # The key of the dictionary is the placeholder node of the graph to be fed,
    # and the value is the numpy array to feed to it.
    feed_dict = {X : batch_data, Y : batch_labels, reg_param : reg, keep_param : 1-dropout}
    _, l = session.run( [optimizer, loss], feed_dict=feed_dict )
    if (step % 10000 == 0):
     print("step %i loss: %f" % (step,l), end=",")
    if (step % 100000 == 0):
      print()
      l, v, m, calc = session.run( [valid_loss, diff_v, loss_v,valid_calc] )
      print("Validation loss: %.4f" % l)
      if best == None or l < best:
        tf.train.Saver().save(session, "./05_29_value.ckpt")
        best = l
      #    break
      #else: best = l
          
          
