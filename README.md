# Credit Risk Scoring Engine

A C++ CLI tool that reads customer financial data from a CSV file and scores each customer's credit risk.

## How it works

Each customer is scored using three weighted factors:

- Debt-to-income ratio (40%)
- Credit utilization (35%)
- Late payment rate (25%)

The weighted score is passed through a saturating exponential transform to produce a risk score between 0 and 1, then classified as:

- **LOW** (< 0.33)
- **MEDIUM** (0.33–0.66)
- **HIGH** (> 0.66)

## Build

```bash
make
```

## Run

```bash
./bin/credit_risk --input data/customers.csv --out reports
```

Optional JSON summary:

```bash
./bin/credit_risk --input data/customers.csv --out reports --summary summary.json
```

## Options

| Flag | Description |
|---|---|
| `--input <path>` | Input CSV file (default: `data/customers.csv`) |
| `--out <dir>` | Output directory for per-customer reports (default: `reports`) |
| `--summary <file>` | Optional path to write a JSON summary |
| `--help` | Show usage |

## Output

- A `.txt` report per customer in the output directory
- A summary printed to the console (counts by category, average risk score)
- An optional `summary.json` with aggregate stats and per-customer results

## Clean

```bash
make clean
```
