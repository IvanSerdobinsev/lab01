#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

const float f_pi = 3.14159265359f;

// разбиение Veltkamp-Dekker
void Split(float a, float& a_high, float& a_low) {
    auto c = static_cast<float>((1ul << 11)+1ul);
    a_high = (c*a)+(a-c*a);
    a_low = a - a_high;
}

void TwoMult(float a, float b, float& s, float& t) {
    float a_high, a_low, b_high, b_low;
    Split(a, a_high, a_low);
    Split(b, b_high, b_low);
    s = a * b;
    t = -s + a_high * b_high;
    t += a_high * b_low;
    t += a_low * b_high;
    t += a_low * b_low;
}

void TwoSum(float a, float b, float& s, float& t) {
    s = a + b;
    float z = s - a;
    t = (a - (s - z)) + (b - z);
}

float my_fma(float a, float b, float c) {
    float prod, err_prod;
    TwoMult(a, b, prod, err_prod);
    float sum, err_sum;
    TwoSum(prod, c, sum, err_sum);
    return sum + (err_prod + err_sum);
}

float integrate(float dv, float v_max, float T, 
                float (*method)(const vector<float>&, float)) {
    int n = static_cast<int>(2 * v_max / dv) + 1;
    vector<float> values(n);
    for (int i = 0; i < n; ++i) {
        float v = -v_max + i * dv;
        values[i] = fabs(v) * (exp(-v * v / T) / sqrt(f_pi * T));
    }
    return method(values, dv);
}
float integrate_with_fma(float dv, float v_max, float T) {
    int n = static_cast<int>(2 * v_max / dv) + 1;
    float sum = 0.0f;
    
    for (int i = 0; i < n; ++i) {
        float v = -v_max + i * dv;
        float pdf = exp(-v * v / T) / sqrt(f_pi * T);

        sum = my_fma(fabs(v), pdf, sum);
    }
    
    return sum * dv;
}

float sum_naive(const vector<float>& vals, float dv) {
    float s = 0;
    for (size_t i = 0; i < vals.size(); ++i) s += vals[i];
    return s * dv;
}

float sum_kahan(const vector<float>& vals, float dv) {
    float sum = 0.f, comp = 0.f;
    for (size_t i = 0; i < vals.size(); ++i) {
        float x = vals[i];
        float y = x - comp;
        float z = sum + y;
        comp = (z - sum) - y;
        sum = z;
    }
    return sum * dv;
}



float sum_double(const vector<float>& vals, float dv) {
    double s = 0;
    for (size_t i = 0; i < vals.size(); ++i) s += vals[i];
    return static_cast<float>(s * dv);
}

int main() {
    float dv = 0.001f;
    
    vector<float> Ts;
    Ts.push_back(0.1f);
    Ts.push_back(1.0f);
    Ts.push_back(100.0f);
    Ts.push_back(10000.0f);
    
    cout << setprecision(8);
    cout << "  T   |   analyt |     Naive |     Kahan  |     my_fma |     Double     |   ε_naive |      ε_kahan |      ε_fma   |      ε_double\n";
    for (size_t idx = 0; idx < Ts.size(); ++idx) { 
        float T = Ts[idx];
        float v_max = 8.0f * sqrt(T);  
    
        
        float analytic = sqrt(T / f_pi);
        
        float naive = integrate(dv, v_max, T, sum_naive);
        float kahan = integrate(dv, v_max, T, sum_kahan);
        float fma = integrate_with_fma(dv, v_max, T);
        float dbl = integrate(dv, v_max, T, sum_double);
        
        float eps_naive = fabs((naive - analytic) / analytic)*pow(10,6);
        float eps_kahan = fabs((kahan - analytic) / analytic)*pow(10,6);
        float eps_fma = fabs((fma - analytic) / analytic)*pow(10,6);
        float eps_double = fabs((dbl - analytic) / analytic)*pow(10,6);
        
        cout << fixed << setprecision(1) << T << "  |"
            <<  setprecision(8)
            << " " << analytic << " |"
            << " " << naive << " |"
            << " " << kahan << " |"
            << " " << fma << " |"
            << " " << dbl << " |"
            << setprecision(12) << eps_naive << " |"
            << " " << eps_kahan << " |"
            << " " << eps_fma << " |"
            << " " << eps_double << "\n";
    }
}
