#  Software Metrics Analyzer

A C++-based tool that analyzes a C++ source file to compute **Halstead Metrics** and **Live Variables**, exports results to structured files, and visualizes them on a clean **HTML dashboard**.

---

##  Features

-  **Halstead Metrics Calculation**
  - Measures program complexity and effort:
    - `n1`, `n2`: distinct operators & operands  
    - `N1`, `N2`: total occurrences  
    - `Vocabulary`, `Length`, `Volume`, `Difficulty`, `Effort`

-  **Live Variable Analysis**
  - Tracks which variables are *live* at each program line (used before being redefined).
  - Helps identify memory optimization and dead variable regions.

-  **Automatic Reporting**
  - Outputs metrics in `metrics.csv`
  - Exports live variable data in `live_variables.json`
  - Updates the dashboard in real-time.

-  **Interactive Dashboard**
  - Built with **HTML, CSS, and JavaScript**
  - Reads data from CSV/JSON files
  - Displays metrics and live variable tables dynamically

---

##  How It Works

1. **Input:**  
   Provide a C++ file (`sample.cpp`) as input.

2. **Analysis:**  
   - Tokenizes the source code into operators, operands, keywords, and identifiers.
   - Calculates Halstead metrics from frequency counts.
   - Performs live variable analysis through backward data-flow tracking.

3. **Output:**  
   - Writes metrics to `metrics.csv`
   - Saves live variable data to `live_variables.json`
   - The HTML dashboard loads and visualizes both files automatically.

---

## Technologies Used

- **C++** — Core logic for parsing and metrics computation  
- **Regex & STL** — For tokenization and data structures  
- **CSV/JSON File Handling** — To store metric data  
- **HTML + JavaScript** — For dashboard visualization  

## Usage 
- Compile the tool - g++ sqmMetrics.cpp -o sqmMetrics
- Run the analyzer - sqmMetrics.exe
---

## 🧩 Example Output

**metrics.csv**
```csv
Metric,Value
n1,15
n2,10
N1,30
N2,25
Vocabulary,25
Length,55
Volume,255.6
Difficulty,22.5
Effort,5740.2


