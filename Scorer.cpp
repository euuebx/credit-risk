#include "Scorer.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
Scorer::Scorer(double d, double u, double l)
    : debtWeight_(d), utilizationWeight_(u), latePaymentWeight_(l) {
    double total = d + u + l;
    if (d < 0 || u < 0 || l < 0 || total <= 0)
        throw std::invalid_argument("Scoring weights must be non-negative.");
    debtWeight_ /= total; utilizationWeight_ /= total; latePaymentWeight_ /= total;
}
double Scorer::clamp(double v, double lo, double hi) {
    return std::max(lo, std::min(v, hi));
}
std::string Scorer::categoryFor(double s) {
    if (s < .33) return "LOW";
    if (s < .66) return "MEDIUM";
    return "HIGH";
}
ScoreResult Scorer::score(const Customer& c) const {
    double dti = c.annualIncome > 0 ? c.totalDebt / c.annualIncome : 1.0;
    double util = c.creditLimit > 0 ? c.creditUsed / c.creditLimit : 1.0;
    dti = clamp(dti, 0, 1); util = clamp(util, 0, 1);
    double late = clamp(static_cast<double>(c.latePayments) / 12.0, 0, 1);
    double weighted = debtWeight_ * dti + utilizationWeight_ * util + latePaymentWeight_ * late;
    double risk = 1.0 - std::exp(-1.5 * weighted);
    risk = clamp(risk, 0, 1);
    return {risk, dti, util, late, categoryFor(risk)};
}
