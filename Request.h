#ifndef REQUEST_H
#define REQUEST_H

#include <string>


 //Represents a web request with IP addresses and processing requirements
 
struct Request {
    std::string ipIn;      // Incoming IP address
    std::string ipOut;     // Outgoing IP address
    int processingTime;    // Time required to process request (clock cycles)
    char jobType;          // Job type: 'S' for streaming, 'P' for processing
    
    Request() : ipIn(""), ipOut(""), processingTime(0), jobType('P') {}
    
    /**
     * @brief Parameterized constructor
     * @param in Incoming IP address
     * @param out Outgoing IP address
     * @param time Processing time required
     * @param type Job type (S or P)
     */
    Request(std::string in, std::string out, int time, char type)
        : ipIn(in), ipOut(out), processingTime(time), jobType(type) {}
};

#endif 