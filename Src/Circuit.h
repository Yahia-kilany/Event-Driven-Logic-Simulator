#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include "Event.h"
#include "Gates.h"
#include "fstream"


// Circuit Class to hold wires and gates
class Circuit {
private:
    std::map<std::string , Wire*> wires;
    std::priority_queue<Event , std::vector<Event> , CompareEvent> eventQueue;

    void addEvent (Event e) {
        eventQueue.push (e);
    }
    std::vector<Gate*> gates;   //recheck the functionality of the vector
public:
    // Create a wire and add it to the circuit
    Wire* addWire (std::string name) {
        Wire* w = new Wire (name);
        wires[name] = w;
        return w;
    }

    void fillEventQueue (const std::vector<Event>& events) {
        for (const Event& e : events) {
            addEvent (e);
        }
    }
    // Create a gate and add it to the circuit
    void addGate (Gate* gate) {
        gates.push_back (gate);
    }

    // Simulate the circuit by evaluating all gates
    void simulate () {
        
        std::queue<Gate*> gateQueue;
        std::ofstream myfile;
        myfile.open("output.sim");
        if (!myfile.is_open()) {
            std::cerr << "Error: Could not open the output file" << std::endl;
            return;
        }
        while (!eventQueue.empty ()) {
            Event e = eventQueue.top (); // Access the highest-priority event
            myfile<< e;
            wires[e.getName ()]->value = e.getValue (); // Update the wire value
            for (Gate* g : wires[e.getName ()]-> endGates) {
                gateQueue.push (g);
            }

            while (!gateQueue.empty ()) {
                Gate* gate = gateQueue.front ();
                eventQueue.push (gate->evaluate (e));
                gateQueue.pop ();
            }

            eventQueue.pop (); // Remove the processed event
        }
    }

    // Print the values of all wires in the circuit
    void printValues () {
        for (const auto& pair : wires) {
            std::cout << pair.second->name << ": " << pair.second->value << std::endl;
        }
    }

    // Destructor to free memory (optional, but recommended)
    ~Circuit () {
        for (auto& pair : wires) {
            delete pair.second; // Delete each Wire
        }
        for (Gate* gate : gates) {
            delete gate; // Delete each Gate
        }
    }
};

#endif // CIRCUIT_H