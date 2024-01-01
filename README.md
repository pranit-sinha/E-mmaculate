# Overview
Deep learning frameworks tend to employ internal representations equivalent to a Directed Acyclic Graph (see how [TensorFlow does it](https://www.tensorflow.org/guide/intro_to_graphs)), making optimized graph operations important for optimizing the deep learning network. Yang et al. (2021) shows that equality saturation (an idea that originates in the [compiler optimization literature](https://dl.acm.org/doi/10.1145/1480881.1480915)) allows for applying graph rewrites simultaneously rather than sequentially.
We use our existing [E-graph implementation](https://github.com/pranit-sinha/CS-1203/blob/main/E-graph.c) to generate and store rewritings, achieving saturation when a given set of rewrites [generates](https://mathworld.wolfram.com/GroupGenerators.html) the state space of the input program.

In Yang et al., this optimization supposedly leads to a runtime speedup of 9.2% on BERT and 8.9% on VGG-19 - if you have the compute to reproduce these claims, feel free to reach out!

E-graphs turn out to be neat little things - read [about a domain in which they're more frequently used](https://www.sciencedirect.com/science/article/pii/S1571066108002934?via%3Dihub). 

