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
   * @param global_history vector of the branch history. taken = 1, not taken = -1
   * @return branch is predicted to be taken if output >= 0. Not taken if output < 0
   */
  int predict(std::vector<int> global_history)
  {
    int prediction = 0;
    // this is the perceptron calculation y = w_0 + SUM[x_i*w_i]
    // we assume the first element in global_history is 1
    // so instead we can write it as y = SUM[x_i*w_i]
    for(int i = 0; i < global_history.size(); i++)
    {
      prediction += global_history[i] * weights[i];
    }
    return prediction;
  }

  /*
   * Carries out a single training iteration. updates weights vector
   * @param min_confidence limit on whether or not you update the percetron
   * @param global_history vector of all the global history used to predict
   * @param prev_branch_pred previous perceptron output. taken >= 0, not taken < 0
   * @param prev_branch_act actual result from last branch. taken = 1, not take = -1
   */
  void train(int min_confidence, std::vector<int> global_history, int prev_branch_pred, int prev_branch_act)
  {
    // if both elements have the same sign then the prediction was done correctly
    // equal signs being multiplied > 0, different result in a negative value
    bool correct_prediction = ((prev_branch_pred * prev_branch_act) > 0);

    // we train the perceptron if it got the prediction wrong
    // or if it the output isn't above a certain threshold
    if (!correct_prediction || (abs(prev_branch_pred) <= min_confidence))
    {
      for (int i = 0; i < weights.size(); i++)
      {
        // update each weight
        weights[i] = weights[i] + prev_branch_act * global_history[i];
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

