#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>
struct Customer {
    std::string id, name;
    double annualIncome = 0.0, totalDebt = 0.0;
    double creditLimit = 0.0, creditUsed = 0.0;
    int latePayments = 0;
};
#endif
