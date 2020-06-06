/* This code can be easily parallelized, so you can start from multiple points simultaneously, 
and report the "best" minimum found */

#include <algorithm>
#include <iostream>
#include <chrono>
#include <vector>

#define real double

using namespace std;

// Objective function
real func(const vector<real>& x) {
    real y;
    // y = x[0]*x[0]+x[1]*x[1]; // Paraboloid (convex)
    y = 100*(x[1]-x[0]*x[0])*(x[1]-x[0]*x[0])+(1-x[0])*(1-x[0]); // Rosenbrock (non-convex)
    return y;
}

// Returns the number of iterations required to find the minimum
size_t worker(real& min, vector<real>& x0, real (*func)(const vector<real>&)) {
    // Parameters
    real delta = 1; // Stepsize
    real tol = 1e-10; // Tolerance

    size_t n = x0.size();
    vector<real> polls(2*n);

    min = func(x0); // Reference minimum

    size_t iters = 0;
    while (delta > tol) { // Until stepsize becomes too small
        // For each dimension, increase and decrease by delta and evaluate
        for (size_t i = 0, j = 0; i < n; ++i, j += 2) {
            x0[i] += delta;
            polls[j] = func(x0); // 1st evaluation
            x0[i] -= 2*delta;
            polls[j+1] = func(x0); // 2nd evaluation
            x0[i] += delta;
        }

        // Determine best candidate
        auto it = min_element(polls.begin(), polls.end());

        if (*it < min) {
            min = *it; // Redefine the minimum
            auto idx = it - polls.begin(); // Get minimum's index

            if (idx % 2)
                x0[idx/2] -= delta;
            else 
                x0[idx/2] += delta;
        }
        else
            delta /= 2; // Reduce stepsize if poll was unsuccessful

        ++iters;
    }

    return iters;
}

int main() {
    real min;
    vector<real> x0 {5, 5}; // Starting point

    auto start = chrono::system_clock::now();
    auto iters = worker(min, x0, func);
    auto end = chrono::system_clock::now();
    chrono::duration<real> elapsed = end - start;

    cout << "Min: " << min << '\n'
         << "Found at: [ ";
         for (const real& i : x0) cout << i << ' ';
    cout << "]\nElapsed time: " << elapsed.count() << "s\n";
    cout << "Iterations: " << iters << '\n';

    return 0;
}
