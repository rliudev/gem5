#ifndef PERCEPTRON
#define PERCEPTRON

#include <vector>

#include "base/random.hh"

/*
 * Creates a single perceptron object that can do branch prediction with just
 * the global history and a vectore of learning weights
 * Author:  Victor Fu
 *          Thomas Chang
 *          Rach Liu
 *          Daan Leiva
 */

class Perceptron
{
private:
  int size; // the size of weights vector
  std::vector<int> weights; // the weights of the perceptron
public:
  /**
   * Perceptron constructor
   * @param size Number of inputs to the perceptron. Size of weights vector
   */
  Perceptron(int size)
  {
    // initialize weights vector to 0s
    weights.resize(size, 0);
    for (std::vector<int>::iterator it = weights.begin(); it != weights.end() ;++it)
      {
        *it = 0;
      }
  }

  /**
   * Generates a prediction based on the branch history provided
   * @param xn: feature vector input.
   * @return class 1 if >= 0, else class 2 (output < 0).
   */
  int predict(std::vector<int> xn)
  {
    int prediction = 0;
    // this is the perceptron calculation y = w_0 + SUM[x_i*w_i]
    // we assume the first element in xn is 1
    // so instead we can write it as y = SUM[x_i*w_i]
    for(int i = 0; i < xn.size(); i++)
    {
      prediction += xn[i] * weights[i];
    }
    return prediction;
  }

  /*
   * Carries out a single training iteration. updates weights vector
   * @param min_confidence limit on whether or not you update the percetron
   * @param xn  feature input
   * @param prev_prediction     previous perceptron output. class1 >= 0, class2 < 0
   * @param actual_result       actual result for prev prediction. class1 = 1, class0 = -1
   */
  void train(int min_confidence, std::vector<int> xn, int prev_prediction, int actual_result)
  {
    // if both elements have the same sign then the prediction was done correctly
    // equal signs being multiplied > 0, different result in a negative value
    bool correct_prediction = ((prev_prediction * actual_result) > 0);

    // we train the perceptron if it got the prediction wrong
    // or if it the output isn't above a certain threshold
    if (!correct_prediction || (abs(prev_prediction) <= min_confidence))
    {
      for (int i = 0; i < weights.size(); i++)
      {
        // update each weight
        weights[i] = weights[i] + actual_result * xn[i];
        printf("new weight for %d is: %d\n", i, weights[i]);
        // prevent single weight sizes from becoming larger than the confidence
        if (abs(weights[i]) > min_confidence)
        {
          if (weights[i] < 0)
          {
            weights[i] = min_confidence * -1;
          }
          else
          {
            weights[i] = min_confidence;
          }
        }

      }
    }
  }

  /*
   * Resets the weight vector. Gets called by the handler global reset function
   */
  void reset()
  {
    // set the vector back to its original values of 0
    for (std::vector<int>::iterator it = weights.begin(); it != weights.end() ;++it)
    {
      *it = 0;
    }
  }

};

#endif // PERCEPTRON

