#include "CSVReader.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
std::string CSVReader::trim(const std::string& v) {
    size_t a=0,b=v.size();
    while(a<b && std::isspace((unsigned char)v[a])) ++a;
    while(b>a && std::isspace((unsigned char)v[b-1])) --b;
    return v.substr(a,b-a);
}
std::string CSVReader::lower(const std::string& v) {
    std::string r=v;
    for(char& c:r) c=(char)std::tolower((unsigned char)c);
    return r;
}
std::vector<std::string> CSVReader::parseCSVLine(const std::string& line) {
    std::vector<std::string> f; std::string s; bool q=false;
    for(size_t i=0;i<line.size();++i) {
        char c=line[i];
        if(c=='"') {
            if(q && i+1<line.size() && line[i+1]=='"') { s+='"'; ++i; }
            else q=!q;
        } else if(c==',' && !q) { f.push_back(trim(s)); s.clear(); }
        else s+=c;
    }
    if(q) throw std::runtime_error("Unclosed quote in CSV row.");
    f.push_back(trim(s)); return f;
}
std::vector<std::string> CSVReader::splitWhitespace(const std::string& line) {
    std::vector<std::string> f; std::istringstream in(line); std::string s;
    while(in>>s) f.push_back(s); return f;
}
double CSVReader::parseDouble(const std::string& v,const std::string& field,size_t line) {
    std::string s=trim(v); if(s.empty()) return 0;
    try { size_t p=0; double x=std::stod(s,&p); if(p!=s.size()||x<0) throw std::invalid_argument("x"); return x; }
    catch(...) { throw std::runtime_error("Invalid "+field+" on line "+std::to_string(line)+": '"+s+"'"); }
}
int CSVReader::parseInt(const std::string& v,const std::string& field,size_t line) {
    std::string s=trim(v); if(s.empty()) return 0;
    try { size_t p=0; long x=std::stol(s,&p); if(p!=s.size()||x<0||x>1000000) throw std::invalid_argument("x"); return (int)x; }
    catch(...) { throw std::runtime_error("Invalid "+field+" on line "+std::to_string(line)+": '"+s+"'"); }
}
std::vector<Customer> CSVReader::read(const std::string& path) {
    std::ifstream file(path); if(!file) throw std::runtime_error("Could not open input file: "+path);
    std::vector<Customer> out; std::string line; size_t n=0; bool first=true;
    while(std::getline(file,line)) {
        ++n; if(!line.empty()&&line.back()=='\r') line.pop_back(); if(trim(line).empty()) continue;
        auto f=line.find(',')!=std::string::npos ? parseCSVLine(line) : splitWhitespace(line);
        if(first) { std::string h=lower(trim(f[0])); if(h=="id"||h=="customer_id") { first=false; continue; } first=false; }
        if(f.size()<7) f.resize(7); if(f.size()>7) throw std::runtime_error("Too many fields on line "+std::to_string(n)+". Expected 7 fields.");
        Customer c; c.id=trim(f[0]); c.name=trim(f[1]); if(c.id.empty()) throw std::runtime_error("Missing customer ID on line "+std::to_string(n)); if(c.name.empty()) c.name="Unknown";
        c.annualIncome=parseDouble(f[2],"annual income",n); c.totalDebt=parseDouble(f[3],"total debt",n); c.creditLimit=parseDouble(f[4],"credit limit",n); c.creditUsed=parseDouble(f[5],"credit used",n); c.latePayments=parseInt(f[6],"late payments",n); out.push_back(c);
    }
    if(out.empty()) throw std::runtime_error("Input file contains no customer records.");
    return out;
}
