#include "CSVReader.h"
#include "Scorer.h"
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
namespace fs=std::filesystem;
struct Options { std::string input="data/customers.csv", output="reports", summary; };
static void usage(const char* p){ std::cout<<"Usage: "<<p<<" [options]\n\nOptions:\n  --input <path>    Input customer CSV file\n  --out <dir>       Output directory\n  --summary <file>  Optional JSON summary\n  --help            Show help\n"; }
static Options args(int argc,char** argv){
    Options o; for(int i=1;i<argc;++i){ std::string a=argv[i]; if(a=="--help"){usage(argv[0]);std::exit(0);} if(a=="--input"||a=="--out"||a=="--summary"){if(i+1>=argc)throw std::runtime_error("Missing value for "+a); std::string v=argv[++i]; if(v.empty()||v.rfind("--",0)==0)throw std::runtime_error("Invalid value for "+a); if(a=="--input")o.input=v;else if(a=="--out")o.output=v;else o.summary=v;} else throw std::runtime_error("Unknown argument: "+a); } return o;
}
static std::string safe(std::string v){for(char& c:v)if(!std::isalnum((unsigned char)c)&&c!='-'&&c!='_')c='_';return v.empty()?"customer":v;}
static std::string esc(const std::string& v){std::ostringstream o;for(char c:v){if(c=='"')o<<"\\\"";else if(c=='\\')o<<"\\\\";else if(c=='\n')o<<"\\n";else if(c=='\r')o<<"\\r";else if(c=='\t')o<<"\\t";else o<<c;}return o.str();}
static void report(const fs::path& dir,const Customer& c,const ScoreResult& r){
    std::ofstream f(dir/(safe(c.id)+".txt")); if(!f)throw std::runtime_error("Could not create customer report.");
    f<<"CREDIT RISK REPORT\n==================\n\nCustomer ID: "<<c.id<<"\nName: "<<c.name<<"\n\nFinancial Information\n---------------------\n"<<std::fixed<<std::setprecision(2)<<"Annual income:       "<<c.annualIncome<<"\nTotal debt:          "<<c.totalDebt<<"\nCredit limit:        "<<c.creditLimit<<"\nCredit used:         "<<c.creditUsed<<"\nLate payments:       "<<c.latePayments<<"\n\nNormalized Features\n-------------------\n"<<std::setprecision(4)<<"Debt-to-income:      "<<r.debtToIncome<<"\nCredit utilization:  "<<r.creditUtilization<<"\nLate-payment rate:   "<<r.latePaymentRate<<"\n\nRisk Assessment\n---------------\nRisk score:          "<<r.score<<"\nCategory:            "<<r.category<<"\n";
}
static void summary(const std::string& p,const std::vector<Customer>& c,const std::vector<ScoreResult>& r){
    std::ofstream f(p);if(!f)throw std::runtime_error("Could not create summary file: "+p);size_t lo=0,me=0,hi=0;double total=0;for(auto& x:r){total+=x.score;if(x.category=="LOW")++lo;else if(x.category=="MEDIUM")++me;else ++hi;}
    f<<"{\n  \"customer_count\": "<<c.size()<<",\n  \"average_risk_score\": "<<std::fixed<<std::setprecision(4)<<total/r.size()<<",\n  \"categories\": {\n    \"LOW\": "<<lo<<",\n    \"MEDIUM\": "<<me<<",\n    \"HIGH\": "<<hi<<"\n  },\n  \"customers\": [\n";
    for(size_t i=0;i<c.size();++i){f<<"    {\n      \"id\": \""<<esc(c[i].id)<<"\",\n      \"name\": \""<<esc(c[i].name)<<"\",\n      \"risk_score\": "<<r[i].score<<",\n      \"category\": \""<<r[i].category<<"\",\n      \"debt_to_income\": "<<r[i].debtToIncome<<",\n      \"credit_utilization\": "<<r[i].creditUtilization<<",\n      \"late_payment_rate\": "<<r[i].latePaymentRate<<"\n    }"<<(i+1<c.size()?',':' ')<<"\n";} f<<"  ]\n}\n";
}
int main(int argc,char** argv){try{auto o=args(argc,argv);auto cs=CSVReader::read(o.input);fs::create_directories(o.output);Scorer s;std::vector<ScoreResult> rs;size_t lo=0,me=0,hi=0;double total=0;std::cout<<"Credit Risk Scoring Engine\n==========================\nInput:  "<<o.input<<"\nOutput: "<<o.output<<"\n\n";for(auto& c:cs){auto r=s.score(c);rs.push_back(r);report(o.output,c,r);total+=r.score;if(r.category=="LOW")++lo;else if(r.category=="MEDIUM")++me;else ++hi;std::cout<<std::fixed<<std::setprecision(3)<<c.id<<" | "<<c.name<<" | score="<<r.score<<" | "<<r.category<<"\n";}if(!o.summary.empty()){summary(o.summary,cs,rs);std::cout<<"\nJSON summary: "<<o.summary<<"\n";}std::cout<<"\nSummary\n-------\nCustomers processed: "<<cs.size()<<"\nLow risk:            "<<lo<<"\nMedium risk:         "<<me<<"\nHigh risk:           "<<hi<<"\nAverage risk score:  "<<total/rs.size()<<"\n";return 0;}catch(const std::exception& e){std::cerr<<"Error: "<<e.what()<<"\n";return 1;}}
