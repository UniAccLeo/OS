import pandas as pd
import matplotlib.pyplot as plt

# Load CSV data
data = pd.read_csv("results2.csv")  # replace with your actual CSV file path

# List of unique traces
traces = data['trace'].unique()

# Plot for each trace
for trace in traces:
    plt.figure(figsize=(10,6))
    
    trace_data = data[data['trace'] == trace]
    algorithms = trace_data['algorithm'].unique()
    
    for alg in algorithms:
        alg_data = trace_data[trace_data['algorithm'] == alg]
        # Sort by frames to get smooth decreasing curve
        alg_data = alg_data.sort_values('frames')
        plt.plot(alg_data['frames'], alg_data['fault_rate'], marker='o', label=alg)
    
    plt.title(f"Page Fault Rate vs Frames for {trace}")
    plt.xlabel("Number of Frames")
    plt.ylabel("Page Fault Rate")
    plt.xscale('log')  # use log scale if frame numbers vary a lot
    plt.yscale('log')  # optional if you want better view of small values
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.legend()
    plt.show()
    
    # Compute average simulation time per algorithm
    avg_times = trace_data.groupby('algorithm')['sim_time'].mean()
    
    plt.figure(figsize=(8,6))
    avg_times.plot(kind='bar', color=['#1f77b4','#ff7f0e','#2ca02c'])
    
    plt.title(f"Average Simulation Time for {trace}")
    plt.ylabel("Simulation Time (seconds)")
    plt.xlabel("Algorithm")
    plt.xticks(rotation=0)
    plt.grid(axis='y', linestyle="--", linewidth=0.5)
    plt.show()