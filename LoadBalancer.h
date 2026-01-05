#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "WebServer.h"
#include "Request.h"
#include <queue>
#include <vector>
#include <set>
#include <fstream>

/**
 * @brief Manages web servers and request queue for load balancing
 */
class LoadBalancer {
private:
    std::queue<Request> requestQueue;     // Queue of pending requests
    std::vector<WebServer*> servers;      // Active web servers
    std::set<std::string> blockedIPs;     // Blocked IP addresses
    int currentTime;                      // Current clock cycle
    int serverChangeWaitTime;             // Cycles to wait before changing servers
    int lastServerChange;                 // Last time servers were changed
    std::ofstream logFile;                // Log file for output
    
    // Statistics
    int totalRequestsProcessed;           // Total completed requests
    int totalRequestsGenerated;           // Total requests created
    
public:
    /**
     * @brief Constructor
     * @param numServers Initial number of servers
     * @param logFileName Name of log file
     */
    LoadBalancer(int numServers, const std::string& logFileName);
    
    /**
     * @brief Destructor - cleans up servers and closes log
     */
    ~LoadBalancer();
    
    /**
     * @brief Add a request to the queue
     * @param req Request to add
     */
    void addRequest(const Request& req);
    
    /**
     * @brief Generate a random request
     * @return Randomly generated request
     */
    Request generateRequest();
    
    /**
     * @brief Generate a random IP address
     * @return IP address string
     */
    std::string generateIP();
    
    /**
     * @brief Process one clock cycle
     */
    void tick();
    
    /**
     * @brief Distribute requests to available servers
     */
    void distributeRequests();
    
    /**
     * @brief Adjust number of servers based on queue size
     */
    void adjustServers();
    
    /**
     * @brief Add a server to the pool
     */
    void addServer();
    
    /**
     * @brief Remove a server from the pool
     */
    void removeServer();
    
    /**
     * @brief Block an IP address
     * @param ip IP address to block
     */
    void blockIP(const std::string& ip);
    
    /**
     * @brief Check if IP is blocked
     * @param ip IP address to check
     * @return true if blocked, false otherwise
     */
    bool isBlocked(const std::string& ip) const;
    
    /**
     * @brief Get current queue size
     * @return Number of requests in queue
     */
    int getQueueSize() const;
    
    /**
     * @brief Get number of active servers
     * @return Server count
     */
    int getServerCount() const;
    
    /**
     * @brief Log current status
     */
    void logStatus();
    
    /**
     * @brief Get current time
     * @return Current clock cycle
     */
    int getCurrentTime() const;
    
    /**
     * @brief Get total requests processed
     * @return Total completed requests
     */
    int getTotalProcessed() const;
};

#endif // LOADBALANCER_H