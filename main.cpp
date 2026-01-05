#include "LoadBalancer.h"
#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Main function to run load balancer simulation
 * @return 0 on successful completion
 */
int main() {
    int numServers;
    int runTime;
    
    cout << "========================================\n";
    cout << "    Load Balancer Simulation\n";
    cout << "========================================\n\n";
    
    // Get user input for number of servers
    cout << "Enter number of initial web servers: ";
    cin >> numServers;
    
    // Validate input
    while (numServers < 1 || numServers > 50) {
        cout << "Please enter a valid number (1-50): ";
        cin >> numServers;
    }
    
    // Get user input for simulation time
    cout << "Enter simulation time (clock cycles): ";
    cin >> runTime;
    
    // Validate input
    while (runTime < 100 || runTime > 100000) {
        cout << "Please enter a valid time (100-100000): ";
        cin >> runTime;
    }
    
    cout << "\nStarting simulation...\n";
    cout << "Initial servers: " << numServers << "\n";
    cout << "Running for: " << runTime << " clock cycles\n\n";
    
    // Create load balancer
    LoadBalancer lb(numServers, "loadbalancer_log.txt");
    
    // Generate initial queue (servers * 20 requests)
    cout << "Generating initial request queue...\n";
    int initialRequests = numServers * 20;
    for (int i = 0; i < initialRequests; i++) {
        Request req = lb.generateRequest();
        lb.addRequest(req);
    }
    cout << "Initial queue size: " << lb.getQueueSize() << "\n\n";
    
    // Optional: Block some IPs for demonstration
    lb.blockIP("192.168.1.100");
    lb.blockIP("10.0.0.50");
    cout << "Firewall active: 2 IPs blocked\n\n";
    
    // Run simulation
    cout << "Running simulation";
    int displayInterval = runTime / 10; // Show progress 10 times
    
    for (int cycle = 0; cycle < runTime; cycle++) {
        lb.tick();
        
        // Display progress
        if (cycle % displayInterval == 0 && cycle > 0) {
            cout << ".";
            cout.flush();
        }
    }
    
    cout << " Complete!\n\n";
    
    // Display final statistics
    cout << "========================================\n";
    cout << "    Simulation Complete\n";
    cout << "========================================\n";
    cout << "Total clock cycles: " << lb.getCurrentTime() << "\n";
    cout << "Final queue size: " << lb.getQueueSize() << "\n";
    cout << "Final server count: " << lb.getServerCount() << "\n";
    cout << "Total requests processed: " << lb.getTotalProcessed() << "\n";
    
    // Calculate efficiency
    double efficiency = (double)lb.getTotalProcessed() / lb.getCurrentTime() * 100;
    cout << fixed;
    cout.precision(2);
    cout << "Processing efficiency: " << efficiency << " requests/cycle\n";
    
    cout << "\nLog file saved to: loadbalancer_log.txt\n";
    cout << "========================================\n";
    
    return 0;
}