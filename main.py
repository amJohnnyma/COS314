import pandas as pd
import re

# Define the raw text block (shortened here for the example, but should be the full input in actual use)
raw_text = """
1	p3.2.a.txt	3588961391	(19.100000, 24.300000)->(18.650000, 26.200000)->(18.000000, 25.300000)->(19.100000, 24.300000)->	20.000000	6.501912	-0.024519	267
5	p3.4.s.txt	3667155210	(19.100000, 24
"""

# Normalize whitespace and split by separator lines
raw_text_cleaned = re.sub(r"-{10,}", "SPLIT", raw_text)
sections = [s.strip() for s in raw_text_cleaned.split("SPLIT") if s.strip()]

# Column headers for the CSV
columns = [
    "GroupID", "Filename", "RunID", "Path", "Cost", "Time", "Reward", "Seed"
]

# Special summary row columns
summary_columns = [
    "GroupID", "Filename", "RunID", "TotalCost", "TotalDuration", "TotalReward"
]

# Collect all rows
data_rows = []

for section in sections:
    lines = section.split("\n")
    for line in lines:
        parts = line.strip().split("\t")
        if len(parts) == 8:
            data_rows.append(parts)
        elif len(parts) == 5 and parts[2] == parts[1]:  # Summary row (TR)
            group_id = parts[0]
            filename = parts[1]
            run_id = parts[2]
            cost = parts[3].replace("TC", "")
            duration = parts[4].replace("TD", "")
            reward = parts[4].replace("TR", "") if "TR" in parts[4] else None
            data_rows.append([group_id, filename, run_id, "", cost, duration, reward, "SUMMARY"])

# Create DataFrame and save to CSV
df = pd.DataFrame(data_rows, columns=columns)
csv_path = "combined_results.csv"
df.to_csv(csv_path, index=False)
csv_path
