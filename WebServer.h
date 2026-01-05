#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"
#include <string>

/**
 * @brief Represents a web server that processes requests
 */
class WebServer {
private:
    int serverID;                  // Unique identifier for the server
    bool isBusy;                   // Whether server is currently processing
    Request currentRequest;        // Request being processed
    int remainingTime;            // Time remaining for current request
    
public:
    //Constructor 
    WebServer(int id);
    
    /**
     * @brief Check if server is busy
     * @return true if processing a request, false otherwise
     */
    bool busy() const;
    
    /**
     * @brief Assign a request to this server
     * @param req Request to process
     */
    void assignRequest(const Request& req);
    
    /**
     * @brief Process one clock cycle
     * @return true if request completed, false otherwise
     */
    bool tick();
    
    /**
     * @brief Get server ID
     * @return Server identifier
     */
    int getID() const;
    
    /**
     * @brief Get current request information
     * @return Current request being processed
     */
    Request getCurrentRequest() const;
    
    /**
     * @brief Get remaining processing time
     * @return Clock cycles remaining
     */
    int getRemainingTime() const;
};

#endif // WEBSERVER_H