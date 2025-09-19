#include <stdio.h>
#include <stdlib.h>

int main() {
    // List of trace files
    char *traces[] = {"bzip.trace", "gcc.trace", "sixpack.trace", "swim.trace"};
    
    // Replacement algorithms
    char *algos[] = {"rand", "fifo", "lru", "clockAlg"};
    
    // Frame counts to test
    int frames[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192};
    
    int numTraces = sizeof(traces)/sizeof(traces[0]);
    int numAlgos = sizeof(algos)/sizeof(algos[0]);
    int numFrames = sizeof(frames)/sizeof(frames[0]);

    // Write CSV header
    FILE *csv = fopen("results2.csv", "w");
    if(!csv) {
        perror("Cannot open results.csv");
        return 1;
    }
    fprintf(csv, "trace,algorithm,frames,page_faults,fault_rate,hit_rate,sim_time\n");
    fclose(csv);

    // Run memsim for all traces, algorithms, and frame counts
    for(int t = 0; t < numTraces; t++) {
        for(int a = 0; a < numAlgos; a++) {
            for(int f = 0; f < numFrames; f++) {
                char cmd[512];
                snprintf(cmd, sizeof(cmd),
                         "./memsim %s %d %s %s >> results2.csv",
                         traces[t], frames[f], algos[a], "quiet");
                printf("Running: %s\n", cmd);
                int ret = system(cmd);
                if(ret != 0) {
                    fprintf(stderr, "Error running command: %s\n", cmd);
                }
            }
        }
    }

    printf("All simulations done. Results are in results.csv\n");
    return 0;
}
