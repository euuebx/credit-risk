```markdown
# Credit Risk Scoring Engine

A command-line tool that reads customer financial data from a CSV file
and generates credit risk scores and reports.

## Features

- Reads customer data from CSV (handles quoted fields, whitespace, headers)
- Computes a risk score from three normalized features:
  - Debt-to-income (40% weight)
  - Credit utilization (35% weight)
  - Late-payment rate (25% weight)
- Classifies each customer as LOW, MEDIUM, or HIGH risk
- Writes one text report per customer
- Optionally writes a JSON summary
- Prints a console summary

## Requirements

- A C++ compiler (g++, clang++, or MSVC)

## Build

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
    src/Customer.cpp src/Scorer.cpp src/CSVReader.cpp src/main.cpp \
    -o credit_risk.exe
```

## Run

```bash
./credit_risk.exe --input data/customers.csv --out reports
```

With an optional JSON summary:

```bash
./credit_risk.exe --input data/customers.csv --out reports --summary summary.json
```

## Options

| Option | Description | Default |
|---|---|---|
| `--input <path>` | Input customer CSV file | `data/customers.csv` |
| `--out <dir>` | Output directory for reports | `reports` |
| `--summary <file>` | Optional JSON summary output | *(none)* |
| `--help` | Show help | |

## Input Format

```
id,name,annual_income,total_debt,credit_limit,credit_used,late_payments
C001,James Murphy,72000,18000,30000,7500,0
C002,Sarah O'Brien,48000,22000,18000,14500,3
```

## Scoring Model

Each feature is normalized to [0, 1]:

- Debt-to-income = `total_debt / annual_income`
- Credit utilization = `credit_used / credit_limit`
- Late-payment rate = `late_payments / 12`

The weighted sum is passed through a saturating transform:

```
risk = 1 - exp(-1.5 * weighted)
```

| Score | Category |
|---|---|
| < 0.33 | LOW |
| < 0.66 | MEDIUM |
| ≥ 0.66 | HIGH |

## Output

One text report per customer is written to the output directory
(e.g. `reports/C001.txt`), including financial info, normalized features,
and the final risk score and category.
```
