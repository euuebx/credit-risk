#ifndef CSVREADER_H
#define CSVREADER_H
#include "Customer.h"
#include <string>
#include <vector>
class CSVReader {
public:
    static std::vector<Customer> read(const std::string& path);
private:
    static std::vector<std::string> parseCSVLine(const std::string&);
    static std::vector<std::string> splitWhitespace(const std::string&);
    static std::string trim(const std::string&);
    static std::string lower(const std::string&);
    static double parseDouble(const std::string&, const std::string&, std::size_t);
    static int parseInt(const std::string&, const std::string&, std::size_t);
};
#endif
