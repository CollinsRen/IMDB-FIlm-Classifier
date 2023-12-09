#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <stdio.h>
#include <sys/types.h>
//#include <dirent.h>
#include <sstream>
#include <vector>
#include <cmath>
#include <float.h>
#include <limits.h>
#include <map>
#include <random>

using namespace std;
#define g(x) ((1.0/(1.0+exp(-x))))
#define gprime(x) ((g(x)*(1-g(x)))) 
#define N 999

// Print out progress bar
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

//static int c = 0;

void printProgress(double percentage, string info) {
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("%s \r%3d%% [%.*s%*s] ", info.c_str(), val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

// Process words to match words in the vocabulary file
string ProcessWord(string s)
{
    string t;
    // Remove punctuation.
    //cout << s <<c<< endl;
    for (int i = 0; i < s.size(); i++)
        if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || s[i] == ' ')
        {
            t += s[i];
        }
    //cout << t <<c<< endl;
    //c++;
    /*
        else
        {
            //t += s[i];
        }
        */

        // Convert to lower case.
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);// "hello, I like the movie!" --> "hello i like the movie"

    return t;

}

// The feature we use is the number of occurances of a word in the file
vector<double> GetFeature(string filename, unordered_map<string, int>& word_to_idx)
{
    /*
    Args:
    - filename: path name to the data file
    - word_to_idx: a mapping where a word maps to a preceptron weight's index.
    */

    // Open file.
    ifstream in;
    in.open(filename.c_str());
    if (!in.is_open())
    {
        cerr << "File not found: " << filename << endl;
        exit(1);
    }

    // TODO: obtain feature
    // ---- START ENTERING CODE ---- //
    vector<double> feature(word_to_idx.size(), 0);  //return the feature vector with 0 as frequency first; need to match the index with the ones in imdb vocabulary
    string temp;
    //in >> temp;
    getline(in, temp);
    string converted_str = ProcessWord(temp); //removed all punctuation but has space betweeen words
    string word = "";
    for (char i : converted_str)
    {
        if (i == ' ')
        {
            if (word_to_idx.find(word) != word_to_idx.end())
            {
                int index = word_to_idx[word]; //   index of word
                feature.at(index)++;
                //cout << "!";
            }
            word = "";
        }
        else word += i;
    }



    // ENTER CODE HERE //
    // ---- STOP ENTERING CODE ---- //

    return feature;
}

void save_trained_weights(vector<double>& weight)
{
    ofstream output_file("trained_weights");
    ostream_iterator<double> output_iterator(output_file, "\n");
    copy(weight.begin(), weight.end(), output_iterator);
    output_file.close();
}

int main()
{
    // Open vocabulary file
    ifstream dict_file;
    dict_file.open("imdb.vocab");
    if (!dict_file.is_open())
    {
        cerr << "Dictionary not found." << endl;
        exit(1);
    }

    // TODO: Create word to weight index map dictionary based on the imdb.vocab
    // ---- START ENTERING CODE ---- //

    unordered_map<string, int> word_index;
    string curr_word;
    int index = 0;
    while (dict_file >> curr_word)
    {
        word_index.insert({ curr_word,index }); // map from all the words in imdbvocab to unique index, from 0 to size
        index++;
    }


    // ENTER CODE HERE //
    // ---- STOP ENTERING CODE ---- //

    dict_file.close();

    // initialize accuracy output file
    ofstream output_acc_file;
    output_acc_file.open("accuracy.txt");

    // TODO: Initialize perceptron weights and training parameters. 
    // Hint: check parameters in the AND perceptron code example
    // ---- START ENTERING CODE ---- //
    vector<double> weights(index, 0);
    for (int i = 0; i < weights.size(); i++)
    {
        //double rdm;
        //rdm = (rand() % (N + 1) / (double)(N + 1));
        weights[i] = 0.01;
        // cout << weights[i] << " ";
    }  //还没写完 想先随机生成一个double数组来代表每一个word的weight


    // ENTER CODE HERE //
    // ---- STOP ENTERING CODE ---- //
    float alpha = 1;
    int total_epoch = 20;

    // Train the weights
    for (int epoch = 0; epoch < total_epoch; ++epoch)
    {
        // Read in train_list
        string file_name;
        string delimiter = "\t";
        ifstream train_file;
        train_file.open("training_list");
        float line_count = 0.0;
        cout << "Epoch " << epoch;


        // TODO: Update perceptron weights based on each data instance
       // vector<vector<string, int> > train_set;
        while (getline(train_file, file_name))
        {
            // Hints (not required to follow): 
            // (1) retrieve the data and class of the data they are separated by '\t' the delimeter
            // (2) obtain feature 
            // (3) update weights
            //vector<pair<string, int>> result;
            double weighted_sum = 0.0;
            int len = file_name.size();
            int one_zero = file_name[len - 1] - '0';
            string file_to_open = file_name.substr(0, len - 2);

            //result.push_back(make_pair(file_name.substr(0, len - 2), ans ));

            vector<double> feature_vec = GetFeature(file_to_open, word_index); //feature vector is returned for each file

           /* 
            for (double d : feature_vec)
            {
                if (d != 0)
                {
                    cout << d << endl;
                }
            }*/
            


            for (int i = 0; i < feature_vec.size(); i++)
            {
                weighted_sum += weights[i] * feature_vec[i];
                //cout << weighted_sum << endl;
                //everything correct up to this point
            }

            for (int j = 0; j < feature_vec.size(); j++)
            {
                //cout << "weight before: " << weights[j] << " ";
            
                weights[j] -= alpha * ((double)g(weighted_sum) - (double)one_zero) * (double)gprime(weighted_sum) * (double)feature_vec[j];
               // cout << (double)g(weighted_sum) << " " << (double)one_zero << " " << (double)gprime(weighted_sum) << " " << (double)feature_vec[j] << endl;
               // cout << alpha * ((double)g(weighted_sum) - (double)one_zero) * (double)gprime(weighted_sum) * (double)feature_vec[j] << endl;
                //cout << "weight after: " << weights[j] << endl;
            }

            // ---- START ENTERING CODE ---- //
            // ENTER CODE HERE //
            // ---- STOP ENTERING CODE ---- //

            // Output current progress on training with progress bar
            line_count += 1;
            printProgress(line_count / 10000, "Epoch " + to_string(epoch));
        }
        train_file.close();
        cout << endl;

        // Save trained weights
        save_trained_weights(weights);  //SAVE THE UPDATED VECTOR OF WEIGHTS

        // Evaluate trained weights with test data
        ifstream test_file;
        test_file.open("test_list");
        float correct_pred = 0.0; // should update during evaluation
        line_count = 0.0;


        while (getline(test_file, file_name))
        {

            // Hint: similar to training, but without updating the weights. Should also compute prediction results.
            // ---- START ENTERING CODE ---- //
            // ENTER CODE HERE //
            // ---- STOP ENTERING CODE ---- //

            double weighted_sum = 0.0;
            int len = file_name.size();
            int one_zero = file_name[len - 1] - '0';
            string file_to_open = file_name.substr(0, len - 2);

            vector<double> feature_vec = GetFeature(file_to_open, word_index); //feature vector is returned for each file

            for (int i = 0; i < feature_vec.size(); i++)
            {
                weighted_sum += weights[i] * feature_vec[i];
            }
            //cout << weighted_sum << endl;
            // g(x) (1.0 / (1.0 + exp(-x)))

            double res= (double)g(weighted_sum);
            //double res = 1.0 / (1.0 + exp(-weighted_sum));

            //cout << res <<one_zero<<correct_pred<< endl;


            // TODO: Check if the prediction is correct. If so, increase correct_pred by one.
            // ---- START ENTERING CODE ---- //
            int temp = res >= 0.5 ? 1 : 0;
            //cout << temp << ":" << res << endl;
            if (temp == one_zero) correct_pred++;
            // ENTER CODE HERE //
            // ---- STOP ENTERING CODE ---- //

            // Output current progress on evaluation with progress bar
            line_count += 1.0;
            printProgress(line_count / 1000, "Evaluating...");
        }
        test_file.close();

        // Compute prediction accuracy
        float acc = (correct_pred / line_count) * 100.0;
        printf("Accuracy: %.2f \n", acc);
        output_acc_file << "Epoch " << epoch << ": Accuracy = " << acc << "%" << endl;
    }
    output_acc_file.close();
}
