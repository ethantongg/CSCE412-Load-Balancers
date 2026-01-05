#include "WebServer.h"

/**
 * @brief Constructor initializes server in idle state
 * @param id Server identifier
 */
WebServer::WebServer(int id) : serverID(id), isBusy(false), remainingTime(0) {}

/**
 * @brief Check if server is currently processing a request
 * @return true if busy, false if idle
 */
bool WebServer::busy() const {
    return isBusy;
}

/**
 * @brief Assign a new request to this server
 * @param req Request to be processed
 */
void WebServer::assignRequest(const Request& req) {
    currentRequest = req;
    remainingTime = req.processingTime;
    isBusy = true;
}

/**
 * @brief Process one clock cycle of the current request
 * @return true if request completed this cycle, false otherwise
 */
bool WebServer::tick() {
    if (!isBusy) {
        return false;
    }
    
    remainingTime--;
    
    if (remainingTime <= 0) {
        isBusy = false;
        return true; // Request completed
    }
    
    return false;
}

/**
 * @brief Get the server's unique identifier
 * @return Server ID
 */
int WebServer::getID() const {
    return serverID;
}

/**
 * @brief Get the current request being processed
 * @return Current request object
 */
Request WebServer::getCurrentRequest() const {
    return currentRequest;
}

/**
 * @brief Get remaining processing time for current request
 * @return Number of clock cycles remaining
 */
int WebServer::getRemainingTime() const {
    return remainingTime;
}