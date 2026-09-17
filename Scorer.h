#ifndef SCORER_H
#define SCORER_H
#include "Customer.h"
#include <string>
struct ScoreResult {
    double score, debtToIncome, creditUtilization, latePaymentRate;
    std::string category;
};
class Scorer {
public:
    Scorer(double debtWeight = .40, double utilizationWeight = .35,
           double latePaymentWeight = .25);
    ScoreResult score(const Customer&) const;
private:
    double debtWeight_, utilizationWeight_, latePaymentWeight_;
    static double clamp(double, double, double);
    static std::string categoryFor(double);
};
#endif
