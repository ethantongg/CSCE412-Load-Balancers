#include "LoadBalancer.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

/**
 * @brief Constructor initializes load balancer with servers
 * @param numServers Initial number of web servers
 * @param logFileName Name of output log file
 */
//Constructor initializes load balancer with servers
    //numServers Initial number of web servers
//logFileName Name of output log file
 
LoadBalancer::LoadBalancer(int numServers, const std::string& logFileName)
    : currentTime(0), serverChangeWaitTime(100), lastServerChange(0),
      totalRequestsProcessed(0), totalRequestsGenerated(0) {
    
    // Initialize random seed
    srand(time(nullptr));
    
    // Create initial servers
    for (int i = 0; i < numServers; i++) {
        servers.push_back(new WebServer(i));
    }
    
    // Open log file
    logFile.open(logFileName);
    if (logFile.is_open()) {
        logFile << "Load Balancer Log\n";
        logFile << "=================\n";
        logFile << "Starting with " << numServers << " servers\n\n";
    }
}

//Destructor
LoadBalancer::~LoadBalancer() {
    for (auto server : servers) {
        delete server;
    }
    
    if (logFile.is_open()) {
        logFile << "\n\nFinal Statistics:\n";
        logFile << "Total Requests Processed: " << totalRequestsProcessed << "\n";
        logFile << "Total Requests Generated: " << totalRequestsGenerated << "\n";
        logFile.close();
    }
}

/**
 * @brief Generate random IP address
 * @return String representation of IP address
 */
std::string LoadBalancer::generateIP() {
    return std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256);
}

/**
 * @brief Generate a random request with random parameters
 * @return Newly generated request
 */
Request LoadBalancer::generateRequest() {
    std::string ipIn = generateIP();
    std::string ipOut = generateIP();
    int time = (rand() % 200) + 50; // 50-249 cycles
    char type = (rand() % 2 == 0) ? 'S' : 'P';
    
    totalRequestsGenerated++;
    return Request(ipIn, ipOut, time, type);
}

/**
 * @brief Add request to queue if IP not blocked
 * @param req Request to add
 */
void LoadBalancer::addRequest(const Request& req) {
    if (!isBlocked(req.ipIn)) {
        requestQueue.push(req);
    }
}

/**
 * @brief Block an IP address (firewall/DOS prevention)
 * @param ip IP address to block
 */
void LoadBalancer::blockIP(const std::string& ip) {
    blockedIPs.insert(ip);
}

/**
 * @brief Check if IP address is blocked
 * @param ip IP address to check
 * @return true if blocked, false otherwise
 */
bool LoadBalancer::isBlocked(const std::string& ip) const {
    return blockedIPs.find(ip) != blockedIPs.end();
}

/**
 * @brief Distribute requests from queue to available servers
 */
void LoadBalancer::distributeRequests() {
    for (auto server : servers) {
        if (!server->busy() && !requestQueue.empty()) {
            Request req = requestQueue.front();
            requestQueue.pop();
            server->assignRequest(req);
        }
    }
}

/**
 * @brief Adjust server count based on queue size
 */
void LoadBalancer::adjustServers() {
    int queueSize = requestQueue.size();
    int serverCount = servers.size();
    
    // Wait before making changes
    if (currentTime - lastServerChange < serverChangeWaitTime) {
        return;
    }
    
    // Add servers if queue is large
    if (queueSize > 25 * serverCount && serverCount < 50) {
        addServer();
        lastServerChange = currentTime;
        
        if (logFile.is_open()) {
            logFile << "Time " << currentTime << ": Added server. "
                    << "Queue: " << queueSize << ", Servers: " << servers.size() << "\n";
        }
    }
    // Remove servers if queue is small
    else if (queueSize < 15 * serverCount && serverCount > 1) {
        // Only remove if no servers would be left idle
        bool hasIdleServer = false;
        for (auto server : servers) {
            if (!server->busy()) {
                hasIdleServer = true;
                break;
            }
        }
        
        if (hasIdleServer) {
            removeServer();
            lastServerChange = currentTime;
            
            if (logFile.is_open()) {
                logFile << "Time " << currentTime << ": Removed server. "
                        << "Queue: " << queueSize << ", Servers: " << servers.size() << "\n";
            }
        }
    }
}

/**
 * @brief Add a new server to the pool
 */
void LoadBalancer::addServer() {
    int newID = servers.size();
    servers.push_back(new WebServer(newID));
}

/**
 * @brief Remove an idle server from the pool
 */
void LoadBalancer::removeServer() {
    if (servers.empty()) return;
    
    // Find and remove an idle server
    for (auto it = servers.begin(); it != servers.end(); ++it) {
        if (!(*it)->busy()) {
            delete *it;
            servers.erase(it);
            return;
        }
    }
}

/**
 * @brief Process one clock cycle for all servers
 */
void LoadBalancer::tick() {
    currentTime++;
    
    // Process each server
    for (auto server : servers) {
        if (server->busy()) {
            bool completed = server->tick();
            if (completed) {
                totalRequestsProcessed++;
            }
        }
    }
    
    // Randomly add new requests
    if (rand() % 10 < 3) { // 30% chance per cycle
        Request newReq = generateRequest();
        addRequest(newReq);
    }
    
    // Distribute requests to available servers
    distributeRequests();
    
    // Adjust server count
    adjustServers();
    
    // Log status periodically
    if (currentTime % 100 == 0) {
        logStatus();
    }
}

/**
 * @brief Log current status to file
 */
void LoadBalancer::logStatus() {
    if (!logFile.is_open()) return;
    
    logFile << "\n--- Time: " << currentTime << " ---\n";
    logFile << "Queue Size: " << requestQueue.size() << "\n";
    logFile << "Active Servers: " << servers.size() << "\n";
    logFile << "Requests Processed: " << totalRequestsProcessed << "\n";
    
    int busyCount = 0;
    for (auto server : servers) {
        if (server->busy()) {
            busyCount++;
            Request req = server->getCurrentRequest();
            logFile << "  Server " << server->getID() << ": Processing "
                    << req.ipIn << " -> " << req.ipOut
                    << " (Type: " << req.jobType << ", Time left: "
                    << server->getRemainingTime() << ")\n";
        }
    }
    
    logFile << "Busy Servers: " << busyCount << "/" << servers.size() << "\n";
    logFile << std::flush;
}

/**
 * @brief Get current queue size
 * @return Number of pending requests
 */
int LoadBalancer::getQueueSize() const {
    return requestQueue.size();
}

/**
 * @brief Get number of active servers
 * @return Server count
 */
int LoadBalancer::getServerCount() const {
    return servers.size();
}

/**
 * @brief Get current clock cycle
 * @return Current time
 */
int LoadBalancer::getCurrentTime() const {
    return currentTime;
}

/**
 * @brief Get total processed requests
 * @return Total completed requests
 */
int LoadBalancer::getTotalProcessed() const {
    return totalRequestsProcessed;
}