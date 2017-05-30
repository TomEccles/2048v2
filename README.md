Bot to play 2048

Performs Monte Carlo tree search, using:
* A single layer neural net for priors during tree exploration, trained on the previous iteration.
* A single layer neural net for values, trained on the previous iteration. When evaluating a leaf, we take a weighted average of this value and a rollout
* A really stupid rollout function - deterministic moves preferring up-left-right-down.

The tree search is in C++, wrapping Python Tensorflow models. That's probably not a great idea, but compiling Tensorflow in C++ turned out to be hard.

TODO:
* Experiment with different neural nets
* Update automatically, rather than in a big pass with a fresh dataset
* Tune how much we weight the priors and the values properly
* Maybe reimplement in Python :-)